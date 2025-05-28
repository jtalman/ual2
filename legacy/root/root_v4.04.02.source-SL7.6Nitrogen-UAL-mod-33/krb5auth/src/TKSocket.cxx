// @(#)root/krb5auth:$Name:  $:$Id: TKSocket.cxx,v 1.3 2005/02/10 12:47:27 rdm Exp $
// Author: Maarten Ballintijn   27/10/2003

#include "config.h"
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "TKSocket.h"
#include "TSocket.h"
#include "TError.h"


extern "C" {
// missing from "krb5.h"
extern int krb5_net_read(/*IN*/ krb5_context context, int fd,
                         /*OUT*/ char *buf,/*IN*/ int len);

extern int krb5_net_write(/*IN*/ krb5_context context, int fd,
                          const char *buf, int len);
}


ClassImp(TKSocket)


krb5_context TKSocket::fgContext = 0;
krb5_ccache TKSocket::fgCCDef = 0;
krb5_principal TKSocket::fgClient = 0;


TKSocket::TKSocket(TSocket *s)
   : fSocket(s), fServer(0), fAuthContext(0)
{
}


TKSocket::~TKSocket()
{
   krb5_free_principal(fgContext, fServer);
   krb5_auth_con_free(fgContext, fAuthContext);
   delete fSocket;
}


TKSocket *TKSocket::Connect(const char *server, Int_t port)
{
   Int_t rc;

   if (fgContext == 0) {
      rc = krb5_init_context(&fgContext);
      if (rc != 0) {
         ::Error("TKSocket::Connect","while initializing krb5 (%d), %s",
                 rc, error_message(rc));
         return 0;
      }


      rc = krb5_cc_default(fgContext, &fgCCDef);
      if (rc != 0) {
         ::Error("TKSocket::Connect","while getting default credential cache (%d), %s",
                 rc, error_message(rc));
         krb5_free_context(fgContext); fgContext = 0;
         return 0;
      }

      rc = krb5_cc_get_principal(fgContext, fgCCDef, &fgClient);
      if (rc != 0) {
         ::Error("TKSocket::Connect","while getting client principal from %s (%d), %s",
                 krb5_cc_get_name(fgContext,fgCCDef), rc, error_message(rc));
         krb5_cc_close(fgContext,fgCCDef); fgCCDef = 0;
         krb5_free_context(fgContext); fgContext = 0;
         return 0;
      }
   }

   TSocket  *s = new TSocket(server, port);

   if (!s->IsValid()) {
      ::SysError("TKSocket::Connect","Cannot connect to %s:%d", server, port);
      delete s;
      return 0;
   }

   TKSocket *ks = new TKSocket(s);

   rc = krb5_sname_to_principal(fgContext, server, "host", KRB5_NT_SRV_HST, &ks->fServer);
   if (rc != 0) {
      ::Error("TKSocket::Connect","while getting server principal (%d), %s",
              rc, error_message(rc));
      delete ks;
      return 0;
   }

   krb5_data cksum_data;
   cksum_data.data = StrDup(server);
   cksum_data.length = strlen(server);

   krb5_error *err_ret;
   krb5_ap_rep_enc_part *rep_ret;

   int sock = ks->fSocket->GetDescriptor();
   rc = krb5_sendauth(fgContext, &ks->fAuthContext, (krb5_pointer) &sock,
                           "KRB5_TCP_Python_v1.0", fgClient, ks->fServer,
                           AP_OPTS_MUTUAL_REQUIRED,
                           &cksum_data,
                           0,           /* no creds, use ccache instead */
                           fgCCDef, &err_ret, &rep_ret, NULL);

   delete [] cksum_data.data;

   if (rc != 0) {
      ::Error("TKSocket::Connect","while sendauth (%d), %s",
              rc, error_message(rc));
      delete ks;
      return 0;
   }

   return ks;
}


Int_t TKSocket::BlockRead(char *&buf, EEncoding &type)
{
   Int_t rc;
   TDesc desc;
   Int_t fd = fSocket->GetDescriptor();

   rc = krb5_net_read(fgContext, fd, (char *)&desc, sizeof(desc));
   if (rc == 0) errno = ECONNABORTED;

   if (rc <= 0) {
      SysError("BlockRead","reading descriptor (%d), %s",
               rc, error_message(rc));
      return -1;
   }

   type = static_cast<EEncoding>(ntohs(desc.type));

   krb5_data enc;
   enc.length = ntohs(desc.length);
   enc.data = new char[enc.length+1];

   rc = krb5_net_read(fgContext, fd, enc.data, enc.length);
   enc.data[enc.length] = 0;

   if (rc == 0) errno = ECONNABORTED;

   if (rc <= 0) {
      SysError("BlockRead","reading data (%d), %s",
               rc, error_message(rc));
      return -1;
   }

   krb5_data out;
   switch (type) {
   case kNone:
         buf = enc.data;
         rc = enc.length;
         break;
   case kSafe:
         rc = krb5_rd_safe(fgContext, fAuthContext, &enc, &out, 0);
         break;
   case kPriv:
         rc = krb5_rd_priv(fgContext, fAuthContext, &enc, &out, 0);
         break;
   default:
         Error("BlockWrite","unknown encoding type (%d)", type);
         return -1;
   }

   if (type != kNone) {
      // copy data to buffer that is new'ed
      buf = new char[out.length+1];
      memcpy(buf, out.data,out.length);
      buf[out.length] = 0;
      free(out.data);
      delete enc.data;
      rc = out.length;
   }

   return rc;
}


Int_t TKSocket::BlockWrite(const char *buf, Int_t length, EEncoding type)
{
   TDesc desc;
   krb5_data in;
   krb5_data enc;
   Int_t rc;
   in.data = const_cast<char*>(buf);
   in.length = length;

   switch (type) {
   case kNone:
         enc.data = in.data;
         enc.length = in.length;
         break;
   case kSafe:
         rc = krb5_mk_safe(fgContext, fAuthContext, &in, &enc, 0);
         break;
   case kPriv:
         rc = krb5_mk_priv(fgContext, fAuthContext, &in, &enc, 0);
         break;
   default:
         Error("BlockWrite","unknown encoding type (%d)", type);
         return -1;
   }

   desc.length = htons(enc.length);
   desc.type = htons(type);

   Int_t fd = fSocket->GetDescriptor();
   rc = krb5_net_write(fgContext, fd, (char *)&desc, sizeof(desc));
   if (rc <= 0) {
      Error("BlockWrite","writing descriptor (%d), %s",
            rc, error_message(rc));
      return -1;
   }

   rc = krb5_net_write(fgContext, fd, (char *)enc.data, enc.length);
   if (rc <= 0) {
      Error("BlockWrite","writing data (%d), %s",
            rc, error_message(rc));
      return -1;
   }

   if (type != kNone) free(enc.data);

   return rc;
}
