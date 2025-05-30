/*
** 2001 September 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This header file defines the interface that the SQLite library
** presents to client programs.
**
** @(#) $Id: qt/sqlite.h   3.3.4   edited Mar 30 2004 $
*/
#ifndef _SQLITE_H_
#define _SQLITE_H_
#include <stdarg.h>     /* Needed for the definition of va_list */

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

/*
** The version of the SQLite library.
*/
#define SQLITE_VERSION         "2.8.13"

/*
** The version string is also compiled into the library so that a program
** can check to make sure that the lib*.a file and the *.h file are from
** the same version.
*/
extern const char sqlite_version[];

/*
** The SQLITE_UTF8 macro is defined if the library expects to see
** UTF-8 encoded data.  The SQLITE_ISO8859 macro is defined if the
** iso8859 encoded should be used.
*/
#define SQLITE_ISO8859 1

/*
** The following constant holds one of two strings, "UTF-8" or "iso8859",
** depending on which character encoding the SQLite library expects to
** see.  The character encoding makes a difference for the LIKE and GLOB
** operators and for the LENGTH() and SUBSTR() functions.
*/
extern const char sqlite_encoding[];

/*
** Each open sqlite database is represented by an instance of the
** following opaque structure.
*/
typedef struct sqlite sqlite;

/*
** A function to open a new sqlite database.  
**
** If the database does not exist and mode indicates write
** permission, then a new database is created.  If the database
** does not exist and mode does not indicate write permission,
** then the open fails, an error message generated (if errmsg!=0)
** and the function returns 0.
** 
** If mode does not indicates user write permission, then the 
** database is opened read-only.
**
** The Truth:  As currently implemented, all databases are opened
** for writing all the time.  Maybe someday we will provide the
** ability to open a database readonly.  The mode parameters is
** provided in anticipation of that enhancement.
*/
sqlite *sqlite_open(const char *filename, int mode, char **errmsg);

/*
** A function to close the database.
**
** Call this function with a pointer to a structure that was previously
** returned from sqlite_open() and the corresponding database will by closed.
*/
void sqlite_close(sqlite *);

/*
** The type for a callback function.
*/
typedef int (*sqlite_callback)(void*,int,char**, char**);

/*
** A function to executes one or more statements of SQL.
**
** If one or more of the SQL statements are queries, then
** the callback function specified by the 3rd parameter is
** invoked once for each row of the query result.  This callback
** should normally return 0.  If the callback returns a non-zero
** value then the query is aborted, all subsequent SQL statements
** are skipped and the sqlite_exec() function returns the SQLITE_ABORT.
**
** The 4th parameter is an arbitrary pointer that is passed
** to the callback function as its first parameter.
**
** The 2nd parameter to the callback function is the number of
** columns in the query result.  The 3rd parameter to the callback
** is an array of strings holding the values for each column.
** The 4th parameter to the callback is an array of strings holding
** the names of each column.
**
** The callback function may be NULL, even for queries.  A NULL
** callback is not an error.  It just means that no callback
** will be invoked.
**
** If an error occurs while parsing or evaluating the SQL (but
** not while executing the callback) then an appropriate error
** message is written into memory obtained from malloc() and
** *errmsg is made to point to that message.  The calling function
** is responsible for freeing the memory that holds the error
** message.   Use sqlite_freemem() for this.  If errmsg==NULL,
** then no error message is ever written.
**
** The return value is is SQLITE_OK if there are no errors and
** some other return code if there is an error.  The particular
** return value depends on the type of error. 
**
** If the query could not be executed because a database file is
** locked or busy, then this function returns SQLITE_BUSY.  (This
** behavior can be modified somewhat using the sqlite_busy_handler()
** and sqlite_busy_timeout() functions below.)
*/
int sqlite_exec(
  sqlite*,                      /* An open database */
  const char *sql,              /* SQL to be executed */
  sqlite_callback,              /* Callback function */
  void *,                       /* 1st argument to callback function */
  char **errmsg                 /* Error msg written here */
);

/*
** Return values for sqlite_exec() and sqlite_step()
*/
#define SQLITE_OK           0   /* Successful result */
#define SQLITE_ERROR        1   /* SQL error or missing database */
#define SQLITE_INTERNAL     2   /* An internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite_interrupt() */
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* (Internal Only) Table or record not found */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
#define SQLITE_EMPTY       16   /* (Internal Only) Database table is empty */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* Too much data for one row of a table */
#define SQLITE_CONSTRAINT  19   /* Abort due to contraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Auxiliary database format error */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_ROW         100  /* sqlite_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite_step() has finished executing */

/*
** Each entry in an SQLite table has a unique integer key.  (The key is
** the value of the INTEGER PRIMARY KEY column if there is such a column,
** otherwise the key is generated at random.  The unique key is always
** available as the ROWID, OID, or _ROWID_ column.)  The following routine
** returns the integer key of the most recent insert in the database.
**
** This function is similar to the mysql_insert_id() function from MySQL.
*/
int sqlite_last_insert_rowid(sqlite*);

/*
** This function returns the number of database rows that were changed
** (or inserted or deleted) by the most recent called sqlite_exec().
**
** All changes are counted, even if they were later undone by a
** ROLLBACK or ABORT.  Except, changes associated with creating and
** dropping tables are not counted.
**
** If a callback invokes sqlite_exec() recursively, then the changes
** in the inner, recursive call are counted together with the changes
** in the outer call.
**
** SQLite implements the command "DELETE FROM table" without a WHERE clause
** by dropping and recreating the table.  (This is much faster than going
** through and deleting individual elements form the table.)  Because of
** this optimization, the change count for "DELETE FROM table" will be
** zero regardless of the number of elements that were originally in the
** table. To get an accurate count of the number of rows deleted, use
** "DELETE FROM table WHERE 1" instead.
*/
int sqlite_changes(sqlite*);

/*
** This function returns the number of database rows that were changed
** by the last INSERT, UPDATE, or DELETE statment executed by sqlite_exec(),
** or by the last VM to run to completion. The change count is not updated
** by SQL statements other than INSERT, UPDATE or DELETE.
**
** Changes are counted, even if they are later undone by a ROLLBACK or
** ABORT. Changes associated with trigger programs that execute as a
** result of the INSERT, UPDATE, or DELETE statement are not counted.
**
** If a callback invokes sqlite_exec() recursively, then the changes
** in the inner, recursive call are counted together with the changes
** in the outer call.
**
** SQLite implements the command "DELETE FROM table" without a WHERE clause
** by dropping and recreating the table.  (This is much faster than going
** through and deleting individual elements form the table.)  Because of
** this optimization, the change count for "DELETE FROM table" will be
** zero regardless of the number of elements that were originally in the
** table. To get an accurate count of the number of rows deleted, use
** "DELETE FROM table WHERE 1" instead.
**
******* THIS IS AN EXPERIMENTAL API AND IS SUBJECT TO CHANGE ******
*/
int sqlite_last_statement_changes(sqlite*);

/* If the parameter to this routine is one of the return value constants
** defined above, then this routine returns a constant text string which
** descripts (in English) the meaning of the return value.
*/
const char *sqlite_error_string(int);
#define sqliteErrStr sqlite_error_string  /* Legacy. Do not use in new code. */

/* This function causes any pending database operation to abort and
** return at its earliest opportunity.  This routine is typically
** called in response to a user action such as pressing "Cancel"
** or Ctrl-C where the user wants a long query operation to halt
** immediately.
*/
void sqlite_interrupt(sqlite*);


/* This function returns true if the given input string comprises
** one or more complete SQL statements.
**
** The algorithm is simple.  If the last token other than spaces
** and comments is a semicolon, then return true.  otherwise return
** false.
*/
int sqlite_complete(const char *sql);

/*
** This routine identifies a callback function that is invoked
** whenever an attempt is made to open a database table that is
** currently locked by another process or thread.  If the busy callback
** is NULL, then sqlite_exec() returns SQLITE_BUSY immediately if
** it finds a locked table.  If the busy callback is not NULL, then
** sqlite_exec() invokes the callback with three arguments.  The
** second argument is the name of the locked table and the third
** argument is the number of times the table has been busy.  If the
** busy callback returns 0, then sqlite_exec() immediately returns
** SQLITE_BUSY.  If the callback returns non-zero, then sqlite_exec()
** tries to open the table again and the cycle repeats.
**
** The default busy callback is NULL.
**
** Sqlite is re-entrant, so the busy handler may start a new query. 
** (It is not clear why anyone would every want to do this, but it
** is allowed, in theory.)  But the busy handler may not close the
** database.  Closing the database from a busy handler will delete 
** data structures out from under the executing query and will 
** probably result in a coredump.
*/
void sqlite_busy_handler(sqlite*, int(*)(void*,const char*,int), void*);

/*
** This routine sets a busy handler that sleeps for a while when a
** table is locked.  The handler will sleep multiple times until 
** at least "ms" milleseconds of sleeping have been done.  After
** "ms" milleseconds of sleeping, the handler returns 0 which
** causes sqlite_exec() to return SQLITE_BUSY.
**
** Calling this routine with an argument less than or equal to zero
** turns off all busy handlers.
*/
void sqlite_busy_timeout(sqlite*, int ms);

/*
** This next routine is really just a wrapper around sqlite_exec().
** Instead of invoking a user-supplied callback for each row of the
** result, this routine remembers each row of the result in memory
** obtained from malloc(), then returns all of the result after the
** query has finished. 
**
** As an example, suppose the query result where this table:
**
**        Name        | Age
**        -----------------------
**        Alice       | 43
**        Bob         | 28
**        Cindy       | 21
**
** If the 3rd argument were &azResult then after the function returns
** azResult will contain the following data:
**
**        azResult[0] = "Name";
**        azResult[1] = "Age";
**        azResult[2] = "Alice";
**        azResult[3] = "43";
**        azResult[4] = "Bob";
**        azResult[5] = "28";
**        azResult[6] = "Cindy";
**        azResult[7] = "21";
**
** Notice that there is an extra row of data containing the column
** headers.  But the *nrow return value is still 3.  *ncolumn is
** set to 2.  In general, the number of values inserted into azResult
** will be ((*nrow) + 1)*(*ncolumn).
**
** After the calling function has finished using the result, it should 
** pass the result data pointer to sqlite_free_table() in order to 
** release the memory that was malloc-ed.  Because of the way the 
** malloc() happens, the calling function must not try to call 
** malloc() directly.  Only sqlite_free_table() is able to release 
** the memory properly and safely.
**
** The return value of this routine is the same as from sqlite_exec().
*/
int sqlite_get_table(
  sqlite*,               /* An open database */
  const char *sql,       /* SQL to be executed */
  char ***resultp,       /* Result written to a char *[]  that this points to */
  int *nrow,             /* Number of result rows written here */
  int *ncolumn,          /* Number of result columns written here */
  char **errmsg          /* Error msg written here */
);

/*
** Call this routine to free the memory that sqlite_get_table() allocated.
*/
void sqlite_free_table(char **result);

/*
** The following routines are wrappers around sqlite_exec() and
** sqlite_get_table().  The only difference between the routines that
** follow and the originals is that the second argument to the 
** routines that follow is really a printf()-style format
** string describing the SQL to be executed.  Arguments to the format
** string appear at the end of the argument list.
**
** All of the usual printf formatting options apply.  In addition, there
** is a "%q" option.  %q works like %s in that it substitutes a null-terminated
** string from the argument list.  But %q also doubles every '\'' character.
** %q is designed for use inside a string literal.  By doubling each '\''
** character it escapes that character and allows it to be inserted into
** the string.
**
** For example, so some string variable contains text as follows:
**
**      char *zText = "It's a happy day!";
**
** We can use this text in an SQL statement as follows:
**
**      sqlite_exec_printf(db, "INSERT INTO table VALUES('%q')",
**          callback1, 0, 0, zText);
**
** Because the %q format string is used, the '\'' character in zText
** is escaped and the SQL generated is as follows:
**
**      INSERT INTO table1 VALUES('It''s a happy day!')
**
** This is correct.  Had we used %s instead of %q, the generated SQL
** would have looked like this:
**
**      INSERT INTO table1 VALUES('It's a happy day!');
**
** This second example is an SQL syntax error.  As a general rule you
** should always use %q instead of %s when inserting text into a string 
** literal.
*/
int sqlite_exec_printf(
  sqlite*,                      /* An open database */
  const char *sqlFormat,        /* printf-style format string for the SQL */
  sqlite_callback,              /* Callback function */
  void *,                       /* 1st argument to callback function */
  char **errmsg,                /* Error msg written here */
  ...                           /* Arguments to the format string. */
);
int sqlite_exec_vprintf(
  sqlite*,                      /* An open database */
  const char *sqlFormat,        /* printf-style format string for the SQL */
  sqlite_callback,              /* Callback function */
  void *,                       /* 1st argument to callback function */
  char **errmsg,                /* Error msg written here */
  va_list ap                    /* Arguments to the format string. */
);
int sqlite_get_table_printf(
  sqlite*,               /* An open database */
  const char *sqlFormat, /* printf-style format string for the SQL */
  char ***resultp,       /* Result written to a char *[]  that this points to */
  int *nrow,             /* Number of result rows written here */
  int *ncolumn,          /* Number of result columns written here */
  char **errmsg,         /* Error msg written here */
  ...                    /* Arguments to the format string */
);
int sqlite_get_table_vprintf(
  sqlite*,               /* An open database */
  const char *sqlFormat, /* printf-style format string for the SQL */
  char ***resultp,       /* Result written to a char *[]  that this points to */
  int *nrow,             /* Number of result rows written here */
  int *ncolumn,          /* Number of result columns written here */
  char **errmsg,         /* Error msg written here */
  va_list ap             /* Arguments to the format string */
);
char *sqlite_mprintf(const char*,...);
char *sqlite_vmprintf(const char*, va_list);

/*
** Windows systems should call this routine to free memory that
** is returned in the in the errmsg parameter of sqlite_open() when
** SQLite is a DLL.  For some reason, it does not work to call free()
** directly.
*/
void sqlite_freemem(void *p);

/*
** Windows systems need functions to call to return the sqlite_version
** and sqlite_encoding strings.
*/
const char *sqlite_libversion(void);
const char *sqlite_libencoding(void);

/*
** A pointer to the following structure is used to communicate with
** the implementations of user-defined functions.
*/
typedef struct sqlite_func sqlite_func;

/*
** Use the following routines to create new user-defined functions.  See
** the documentation for details.
*/
int sqlite_create_function(
  sqlite*,                  /* Database where the new function is registered */
  const char *zName,        /* Name of the new function */
  int nArg,                 /* Number of arguments.  -1 means any number */
  void (*xFunc)(sqlite_func*,int,const char**),  /* C code to implement */
  void *pUserData           /* Available via the sqlite_user_data() call */
);
int sqlite_create_aggregate(
  sqlite*,                  /* Database where the new function is registered */
  const char *zName,        /* Name of the function */
  int nArg,                 /* Number of arguments */
  void (*xStep)(sqlite_func*,int,const char**), /* Called for each row */
  void (*xFinalize)(sqlite_func*),       /* Called once to get final result */
  void *pUserData           /* Available via the sqlite_user_data() call */
);

/*
** Use the following routine to define the datatype returned by a
** user-defined function.  The second argument can be one of the
** constants SQLITE_NUMERIC, SQLITE_TEXT, or SQLITE_ARGS or it
** can be an integer greater than or equal to zero.  When the datatype
** parameter is non-negative, the type of the result will be the
** same as the datatype-th argument.  If datatype==SQLITE_NUMERIC
** then the result is always numeric.  If datatype==SQLITE_TEXT then
** the result is always text.  If datatype==SQLITE_ARGS then the result
** is numeric if any argument is numeric and is text otherwise.
*/
int sqlite_function_type(
  sqlite *db,               /* The database there the function is registered */
  const char *zName,        /* Name of the function */
  int datatype              /* The datatype for this function */
);
#define SQLITE_NUMERIC     (-1)
#define SQLITE_TEXT        (-2)
#define SQLITE_ARGS        (-3)

/*
** The user function implementations call one of the following four routines
** in order to return their results.  The first parameter to each of these
** routines is a copy of the first argument to xFunc() or xFinialize().
** The second parameter to these routines is the result to be returned.
** A NULL can be passed as the second parameter to sqlite_set_result_string()
** in order to return a NULL result.
**
** The 3rd argument to _string and _error is the number of characters to
** take from the string.  If this argument is negative, then all characters
** up to and including the first '\000' are used.
**
** The sqlite_set_result_string() function allocates a buffer to hold the
** result and returns a pointer to this buffer.  The calling routine
** (that is, the implmentation of a user function) can alter the content
** of this buffer if desired.
*/
char *sqlite_set_result_string(sqlite_func*,const char*,int);
void sqlite_set_result_int(sqlite_func*,int);
void sqlite_set_result_double(sqlite_func*,double);
void sqlite_set_result_error(sqlite_func*,const char*,int);

/*
** The pUserData parameter to the sqlite_create_function() and
** sqlite_create_aggregate() routines used to register user functions
** is available to the implementation of the function using this
** call.
*/
void *sqlite_user_data(sqlite_func*);

/*
** Aggregate functions use the following routine to allocate
** a structure for storing their state.  The first time this routine
** is called for a particular aggregate, a new structure of size nBytes
** is allocated, zeroed, and returned.  On subsequent calls (for the
** same aggregate instance) the same buffer is returned.  The implementation
** of the aggregate can use the returned buffer to accumulate data.
**
** The buffer allocated is freed automatically be SQLite.
*/
void *sqlite_aggregate_context(sqlite_func*, int nBytes);

/*
** The next routine returns the number of calls to xStep for a particular
** aggregate function instance.  The current call to xStep counts so this
** routine always returns at least 1.
*/
int sqlite_aggregate_count(sqlite_func*);

/*
** This routine registers a callback with the SQLite library.  The
** callback is invoked (at compile-time, not at run-time) for each
** attempt to access a column of a table in the database.  The callback
** returns SQLITE_OK if access is allowed, SQLITE_DENY if the entire
** SQL statement should be aborted with an error and SQLITE_IGNORE
** if the column should be treated as a NULL value.
*/
int sqlite_set_authorizer(
  sqlite*,
  int (*xAuth)(void*,int,const char*,const char*,const char*,const char*),
  void *pUserData
);

/*
** The second parameter to the access authorization function above will
** be one of the values below.  These values signify what kind of operation
** is to be authorized.  The 3rd and 4th parameters to the authorization
** function will be parameters or NULL depending on which of the following
** codes is used as the second parameter.  The 5th parameter is the name
** of the database ("main", "temp", etc.) if applicable.  The 6th parameter
** is the name of the inner-most trigger or view that is responsible for
** the access attempt or NULL if this access attempt is directly from 
** input SQL code.
**
**                                          Arg-3           Arg-4
*/
#define SQLITE_COPY                  0   /* Table Name      File Name       */
#define SQLITE_CREATE_INDEX          1   /* Index Name      Table Name      */
#define SQLITE_CREATE_TABLE          2   /* Table Name      NULL            */
#define SQLITE_CREATE_TEMP_INDEX     3   /* Index Name      Table Name      */
#define SQLITE_CREATE_TEMP_TABLE     4   /* Table Name      NULL            */
#define SQLITE_CREATE_TEMP_TRIGGER   5   /* Trigger Name    Table Name      */
#define SQLITE_CREATE_TEMP_VIEW      6   /* View Name       NULL            */
#define SQLITE_CREATE_TRIGGER        7   /* Trigger Name    Table Name      */
#define SQLITE_CREATE_VIEW           8   /* View Name       NULL            */
#define SQLITE_DELETE                9   /* Table Name      NULL            */
#define SQLITE_DROP_INDEX           10   /* Index Name      Table Name      */
#define SQLITE_DROP_TABLE           11   /* Table Name      NULL            */
#define SQLITE_DROP_TEMP_INDEX      12   /* Index Name      Table Name      */
#define SQLITE_DROP_TEMP_TABLE      13   /* Table Name      NULL            */
#define SQLITE_DROP_TEMP_TRIGGER    14   /* Trigger Name    Table Name      */
#define SQLITE_DROP_TEMP_VIEW       15   /* View Name       NULL            */
#define SQLITE_DROP_TRIGGER         16   /* Trigger Name    Table Name      */
#define SQLITE_DROP_VIEW            17   /* View Name       NULL            */
#define SQLITE_INSERT               18   /* Table Name      NULL            */
#define SQLITE_PRAGMA               19   /* Pragma Name     1st arg or NULL */
#define SQLITE_READ                 20   /* Table Name      Column Name     */
#define SQLITE_SELECT               21   /* NULL            NULL            */
#define SQLITE_TRANSACTION          22   /* NULL            NULL            */
#define SQLITE_UPDATE               23   /* Table Name      Column Name     */
#define SQLITE_ATTACH               24   /* Filename        NULL            */
#define SQLITE_DETACH               25   /* Database Name   NULL            */


/*
** The return value of the authorization function should be one of the
** following constants:
*/
/* #define SQLITE_OK  0   // Allow access (This is actually defined above) */
#define SQLITE_DENY   1   /* Abort the SQL statement with an error */
#define SQLITE_IGNORE 2   /* Don't allow access, but don't generate an error */

/*
** Register a function that is called at every invocation of sqlite_exec()
** or sqlite_compile().  This function can be used (for example) to generate
** a log file of all SQL executed against a database.
*/
void *sqlite_trace(sqlite*, void(*xTrace)(void*,const char*), void*);

/*** The Callback-Free API
** 
** The following routines implement a new way to access SQLite that does not
** involve the use of callbacks.
**
** An sqlite_vm is an opaque object that represents a single SQL statement
** that is ready to be executed.
*/
typedef struct sqlite_vm sqlite_vm;

/*
** To execute an SQLite query without the use of callbacks, you first have
** to compile the SQL using this routine.  The 1st parameter "db" is a pointer
** to an sqlite object obtained from sqlite_open().  The 2nd parameter
** "zSql" is the text of the SQL to be compiled.   The remaining parameters
** are all outputs.
**
** *pzTail is made to point to the first character past the end of the first
** SQL statement in zSql.  This routine only compiles the first statement
** in zSql, so *pzTail is left pointing to what remains uncompiled.
**
** *ppVm is left pointing to a "virtual machine" that can be used to execute
** the compiled statement.  Or if there is an error, *ppVm may be set to NULL.
** If the input text contained no SQL (if the input is and empty string or
** a comment) then *ppVm is set to NULL.
**
** If any errors are detected during compilation, an error message is written
** into space obtained from malloc() and *pzErrMsg is made to point to that
** error message.  The calling routine is responsible for freeing the text
** of this message when it has finished with it.  Use sqlite_freemem() to
** free the message.  pzErrMsg may be NULL in which case no error message
** will be generated.
**
** On success, SQLITE_OK is returned.  Otherwise and error code is returned.
*/
int sqlite_compile(
  sqlite *db,                   /* The open database */
  const char *zSql,             /* SQL statement to be compiled */
  const char **pzTail,          /* OUT: uncompiled tail of zSql */
  sqlite_vm **ppVm,             /* OUT: the virtual machine to execute zSql */
  char **pzErrmsg               /* OUT: Error message. */
);

/*
** After an SQL statement has been compiled, it is handed to this routine
** to be executed.  This routine executes the statement as far as it can
** go then returns.  The return value will be one of SQLITE_DONE,
** SQLITE_ERROR, SQLITE_BUSY, SQLITE_ROW, or SQLITE_MISUSE.
**
** SQLITE_DONE means that the execute of the SQL statement is complete
** an no errors have occurred.  sqlite_step() should not be called again
** for the same virtual machine.  *pN is set to the number of columns in
** the result set and *pazColName is set to an array of strings that
** describe the column names and datatypes.  The name of the i-th column
** is (*pazColName)[i] and the datatype of the i-th column is
** (*pazColName)[i+*pN].  *pazValue is set to NULL.
**
** SQLITE_ERROR means that the virtual machine encountered a run-time
** error.  sqlite_step() should not be called again for the same
** virtual machine.  *pN is set to 0 and *pazColName and *pazValue are set
** to NULL.  Use sqlite_finalize() to obtain the specific error code
** and the error message text for the error.
**
** SQLITE_BUSY means that an attempt to open the database failed because
** another thread or process is holding a lock.  The calling routine
** can try again to open the database by calling sqlite_step() again.
** The return code will only be SQLITE_BUSY if no busy handler is registered
** using the sqlite_busy_handler() or sqlite_busy_timeout() routines.  If
** a busy handler callback has been registered but returns 0, then this
** routine will return SQLITE_ERROR and sqltie_finalize() will return
** SQLITE_BUSY when it is called.
**
** SQLITE_ROW means that a single row of the result is now available.
** The data is contained in *pazValue.  The value of the i-th column is
** (*azValue)[i].  *pN and *pazColName are set as described in SQLITE_DONE.
** Invoke sqlite_step() again to advance to the next row.
**
** SQLITE_MISUSE is returned if sqlite_step() is called incorrectly.
** For example, if you call sqlite_step() after the virtual machine
** has halted (after a prior call to sqlite_step() has returned SQLITE_DONE)
** or if you call sqlite_step() with an incorrectly initialized virtual
** machine or a virtual machine that has been deleted or that is associated
** with an sqlite structure that has been closed.
*/
int sqlite_step(
  sqlite_vm *pVm,              /* The virtual machine to execute */
  int *pN,                     /* OUT: Number of columns in result */
  const char ***pazValue,      /* OUT: Column data */
  const char ***pazColName     /* OUT: Column names and datatypes */
);

/*
** This routine is called to delete a virtual machine after it has finished
** executing.  The return value is the result code.  SQLITE_OK is returned
** if the statement executed successfully and some other value is returned if
** there was any kind of error.  If an error occurred and pzErrMsg is not
** NULL, then an error message is written into memory obtained from malloc()
** and *pzErrMsg is made to point to that error message.  The calling routine
** should use sqlite_freemem() to delete this message when it has finished
** with it.
**
** This routine can be called at any point during the execution of the
** virtual machine.  If the virtual machine has not completed execution
** when this routine is called, that is like encountering an error or
** an interrupt.  (See sqlite_interrupt().)  Incomplete updates may be
** rolled back and transactions cancelled,  depending on the circumstances,
** and the result code returned will be SQLITE_ABORT.
*/
int sqlite_finalize(sqlite_vm*, char **pzErrMsg);

/*
** This routine deletes the virtual machine, writes any error message to
** *pzErrMsg and returns an SQLite return code in the same way as the
** sqlite_finalize() function.
**
** Additionally, if ppVm is not NULL, *ppVm is left pointing to a new virtual
** machine loaded with the compiled version of the original query ready for
** execution.
**
** If sqlite_reset() returns SQLITE_SCHEMA, then *ppVm is set to NULL.
**
******* THIS IS AN EXPERIMENTAL API AND IS SUBJECT TO CHANGE ******
*/
int sqlite_reset(sqlite_vm*, char **pzErrMsg);

/*
** If the SQL that was handed to sqlite_compile contains variables that
** are represeted in the SQL text by a question mark ('?').  This routine
** is used to assign values to those variables.
**
** The first parameter is a virtual machine obtained from sqlite_compile().
** The 2nd "idx" parameter determines which variable in the SQL statement
** to bind the value to.  The left most '?' is 1.  The 3rd parameter is
** the value to assign to that variable.  The 4th parameter is the number
** of bytes in the value, including the terminating \000 for strings.
** Finally, the 5th "copy" parameter is TRUE if SQLite should make its
** own private copy of this value, or false if the space that the 3rd
** parameter points to will be unchanging and can be used directly by
** SQLite.
**
** Unbound variables are treated as having a value of NULL.  To explicitly
** set a variable to NULL, call this routine with the 3rd parameter as a
** NULL pointer.
**
** If the 4th "len" parameter is -1, then strlen() is used to find the
** length.
**
** This routine can only be called immediately after sqlite_compile()
** or sqlite_reset() and before any calls to sqlite_step().
**
******* THIS IS AN EXPERIMENTAL API AND IS SUBJECT TO CHANGE ******
*/
int sqlite_bind(sqlite_vm*, int idx, const char *value, int len, int copy);

/*
** This routine configures a callback function - the progress callback - that
** is invoked periodically during long running calls to sqlite_exec(),
** sqlite_step() and sqlite_get_table(). An example use for this API is to keep
** a GUI updated during a large query.
**
** The progress callback is invoked once for every N virtual machine opcodes,
** where N is the second argument to this function. The progress callback
** itself is identified by the third argument to this function. The fourth
** argument to this function is a void pointer passed to the progress callback
** function each time it is invoked.
**
** If a call to sqlite_exec(), sqlite_step() or sqlite_get_table() results 
** in less than N opcodes being executed, then the progress callback is not
** invoked.
** 
** Calling this routine overwrites any previously installed progress callback.
** To remove the progress callback altogether, pass NULL as the third
** argument to this function.
**
** If the progress callback returns a result other than 0, then the current 
** query is immediately terminated and any database changes rolled back. If the
** query was part of a larger transaction, then the transaction is not rolled
** back and remains active. The sqlite_exec() call returns SQLITE_ABORT. 
**
******* THIS IS AN EXPERIMENTAL API AND IS SUBJECT TO CHANGE ******
*/
void sqlite_progress_handler(sqlite*, int, int(*)(void*), void*);

/*
** Register a callback function to be invoked whenever a new transaction
** is committed.  The pArg argument is passed through to the callback.
** callback.  If the callback function returns non-zero, then the commit
** is converted into a rollback.
**
** If another function was previously registered, its pArg value is returned.
** Otherwise NULL is returned.
**
** Registering a NULL function disables the callback.
**
******* THIS IS AN EXPERIMENTAL API AND IS SUBJECT TO CHANGE ******
*/
void *sqlite_commit_hook(sqlite*, int(*)(void*), void*);

/*
** Open an encrypted SQLite database.  If pKey==0 or nKey==0, this routine
** is the same as sqlite_open().
**
** The code to implement this API is not available in the public release
** of SQLite.
*/
sqlite *sqlite_open_encrypted(
  const char *zFilename,   /* Name of the encrypted database */
  const void *pKey,        /* Pointer to the key */
  int nKey,                /* Number of bytes in the key */
  int *pErrcode,           /* Write error code here */
  char **pzErrmsg          /* Write error message here */
);

/*
** Change the key on an open database.  If the current database is not
** encrypted, this routine will encrypt it.  If pNew==0 or nNew==0, the
** database is decrypted.
**
** The code to implement this API is not available in the public release
** of SQLite.
*/
int sqlite_rekey(
  sqlite *db,                    /* Database to be rekeyed */
  const void *pKey, int nKey     /* The new key */
);

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif /* _SQLITE_H_ */
