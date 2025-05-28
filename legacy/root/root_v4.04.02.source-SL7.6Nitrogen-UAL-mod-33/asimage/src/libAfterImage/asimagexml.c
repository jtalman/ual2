/*
 * Copyright (c) 2001 Sasha Vasko <sasha@aftercode.net>
 * Copyright (c) 2001 Eric Kowalski <eric@beancrock.net>
 * Copyright (c) 2001 Ethan Fisher <allanon@crystaltokyo.com>
 *
 * This module is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#undef  LOCAL_DEBUG
#ifdef _WIN32
#include "win32/config.h"
#else
#include "config.h"
#endif

#include <ctype.h>
#include <math.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#include <string.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#ifndef _WIN32
#include <sys/times.h>
#endif

#ifdef _WIN32
# include "win32/afterbase.h"
#else
# include "afterbase.h"
#endif
#include "afterimage.h"
#include "imencdec.h"
#include "pixmap.h" /* for GetRootDimensions() */

/****h* libAfterImage/asimagexml
 * NAME
 * ascompose is a tool to compose image(s) and display/save it based on
 * supplied XML input file.
 *
 * DESCRIPTION
 * ascompose reads supplied XML data, and manipulates image accordingly.
 * It could transform images from files of any supported file format,
 * draw gradients, render antialiased texturized text, perform
 * superimposition of arbitrary number of images, and save images into
 * files of any of supported output file formats.
 *
 * At any point, the result of any operation could be assigned a name,
 * and later on referenced under this name.
 *
 * At any point during the script processing, result of any operation
 * could be saved into a file of any supported file types.
 *
 * Internal image format is 32bit ARGB with 8bit per channel.
 *
 * Last image referenced, will be displayed in X window, unless -n option
 * is specified. If -r option is specified, then this image will be
 * displayed in root window of X display, effectively setting a background
 * for a desktop. If -o option is specified, this image will also be
 * saved into the file or requested type.
 *
 * TAGS
 * 
 * Here is the list and description of possible XML tags to use in the
 * script :
 * 	img       - load image from the file.
 * 	recall    - recall previously loaded/generated image by its name.
 * 	text      - render text string into new image.
 * 	save      - save an image into the file.
 * 	bevel     - draw solid bevel frame around the image.
 * 	gradient  - render multipoint gradient.
 * 	mirror    - create mirror copy of an image.
 * 	blur      - perform gaussian blur on an image.
 * 	rotate    - rotate/flip image in 90 degree increments.
 * 	scale     - scale an image to arbitrary size.
 * 	crop      - crop an image to arbitrary size.
 * 	tile      - tile an image to arbitrary size.
 * 	hsv       - adjust Hue, Saturation and Value of an image.
 * 	pad       - pad image with solid color from either or all sides.
 * 	solid     - generate new image of requested size, filled with solid
 *              color.
 * 	composite - superimpose arbitrary number of images using one of 15
 *              available methods.
 *
 * Each tag generates new image as the result of the transformation -
 * existing images are never modified and could be reused as many times
 * as needed. See below for description of each tag.
 *
 * Whenever numerical values are involved, the basic math ops (add,
 * subtract, multiply, divide), unary minus, and parentheses are
 * supported.
 *
 * Operator precedence is NOT supported.  Percentages are allowed, and
 * apply to either width or height of the appropriate image (usually
 * the refid image).
 *
 * Also, variables of the form $image.width and $image.height are
 * supported.  $image.width is the width of the image with refid "image",
 * and $image.height is the height of the same image.  The special
 * $xroot.width and $xroot.height values are defined by the the X root
 * window, if there is one.  This allows images to be scaled to the
 * desktop size: <scale width="$xroot.width" height="$xroot.height">.
 *
 * Each tag is only allowed to return ONE image.
 *
* 
 *****/

char *interpret_ctrl_codes( char *text );

static char* cdata_str = XML_CDATA_STR;
static char* container_str = XML_CONTAINER_STR;
static ASHashTable *asxml_var = NULL;

static ASImageManager *_as_xml_image_manager = NULL ;
static ASFontManager *_as_xml_font_manager = NULL ;

void set_xml_image_manager( ASImageManager *imman )
{
	_as_xml_image_manager = imman ;
}
void set_xml_font_manager( ASFontManager *fontman )
{
	_as_xml_font_manager = fontman ;
}


void asxml_var_insert(const char* name, int value);

void
asxml_var_init(void)
{
	int w, h;
	if ( asxml_var == NULL )
	{
    	asxml_var = create_ashash(0, string_hash_value, string_compare, string_destroy_without_data);
    	if (!asxml_var) return;
    	if (dpy != NULL && GetRootDimensions(&w, &h))
		{
        	asxml_var_insert("xroot.width", w);
        	asxml_var_insert("xroot.height", h);
      	}
	}
}

void
asxml_var_insert(const char* name, int value)
{
	ASHashData hdata;

    if (!asxml_var) asxml_var_init();
    if (!asxml_var) return;

    /* Destroy any old data associated with this name. */
    remove_hash_item(asxml_var, AS_HASHABLE(name), NULL, True);

    show_progress("Defining var [%s] == %d.", name, value);

    hdata.i = value;
    add_hash_item(asxml_var, AS_HASHABLE(mystrdup(name)), hdata.vptr);
}

int
asxml_var_get(const char* name)
{
	ASHashData hdata = {0};

    if (!asxml_var) asxml_var_init();
    if (!asxml_var) return 0;
    if( get_hash_item(asxml_var, AS_HASHABLE(name), &hdata.vptr) != ASH_Success ) 
	{	
		show_debug(__FILE__, "asxml_var_get", __LINE__, "Use of undefined variable [%s].", name);
		return 0;
	}
    return hdata.i;
}

int
asxml_var_nget(char* name, int n) {
      int value;
      char oldc = name[n];
      name[n] = '\0';
      value = asxml_var_get(name);
      name[n] = oldc;
      return value;
}

void
asxml_var_cleanup(void)
{
	if ( asxml_var != NULL )
    	destroy_ashash( &asxml_var );

}


ASImageManager *create_generic_imageman(const char *path)		
{
	ASImageManager *my_imman = NULL ;
	char *path2 = copy_replace_envvar( getenv( ASIMAGE_PATH_ENVVAR ) );
	show_progress("image path is \"%s\".", path2 );
	if( path != NULL )
		my_imman = create_image_manager( NULL, SCREEN_GAMMA, path, path2, NULL );
	else
		my_imman = create_image_manager( NULL, SCREEN_GAMMA, path2, NULL );
	LOCAL_DEBUG_OUT( "created image manager %p with search path \"%s\"", my_imman, my_imman->search_path[0] );
	if( path2 )
		free( path2 );
	return my_imman;
}

ASFontManager *create_generic_fontman(Display *dpy, const char *path)		   
{
	ASFontManager  *my_fontman ;
	char *path2 = copy_replace_envvar( getenv( ASFONT_PATH_ENVVAR ) );
	if( path != NULL )
	{
		if( path2 != NULL )
		{
			int path_len = strlen(path);
			char *full_path = safemalloc( path_len+1+strlen(path2)+1);
			strcpy( full_path, path );
			full_path[path_len] = ':';
			strcpy( &(full_path[path_len+1]), path2 );
			free( path2 );
			path2 = full_path ;
		}else
			path2 = (char*)path ;
	}
	my_fontman = create_font_manager( dpy, path2, NULL );
	if( path2 && path2 != path )
		free( path2 );

	return my_fontman;
}

ASImage *
compose_asimage_xml(ASVisual *asv, ASImageManager *imman, ASFontManager *fontman, char *doc_str, ASFlagType flags, int verbose, Window display_win, const char *path)
{
	ASImage* im = NULL;
	xml_elem_t* doc;
	ASImageManager *my_imman = imman, *old_as_xml_imman = _as_xml_image_manager ;
	ASFontManager  *my_fontman = fontman, *old_as_xml_fontman = _as_xml_font_manager ; ;

    asxml_var_init();

	doc = xml_parse_doc(doc_str, NULL);
	if (verbose > 1) 
	{
		xml_print(doc);
		fprintf(stderr, "\n");
	}

	/* Build the image(s) from the xml document structure. */
	if (doc)
	{
		xml_elem_t* ptr;
		if( my_imman == NULL )
		{	
			if( _as_xml_image_manager == NULL )
				_as_xml_image_manager = create_generic_imageman( path );/* we'll want to reuse it in case of recursion */
			my_imman = _as_xml_image_manager ;
		}

		if( my_fontman == NULL )
		{
			if( _as_xml_font_manager == NULL )
				_as_xml_font_manager = create_generic_fontman( asv->dpy, path );
			my_fontman = _as_xml_font_manager ;
		}
		
		for (ptr = doc->child ; ptr ; ptr = ptr->next) {
			ASImage* tmpim = build_image_from_xml(asv, my_imman, my_fontman, ptr, NULL, flags, verbose, display_win);
			if (tmpim && im) safe_asimage_destroy(im);
			if (tmpim) im = tmpim;
		}
LOCAL_DEBUG_OUT( "result im = %p, im->imman	= %p, my_imman = %p, im->magic = %8.8lX", im, im?im->imageman:NULL, my_imman, im?im->magic:0 );

		if( my_imman != imman && my_imman != old_as_xml_imman )
		{/* detach created image from imman to be destroyed : */
			if( im && im->imageman == my_imman )
				forget_asimage( im );
			destroy_image_manager(my_imman, False);
		}
		if( my_fontman != fontman && my_fontman != old_as_xml_fontman  )
			destroy_font_manager(my_fontman, False);
		/* must restore managers to its original state */
		_as_xml_image_manager = old_as_xml_imman   ;
		_as_xml_font_manager =  old_as_xml_fontman ;
		
		/* Delete the xml. */
		xml_elem_delete(NULL, doc);
	}
	LOCAL_DEBUG_OUT( "returning im = %p, im->imman	= %p, im->magic = %8.8lX", im, im?im->imageman:NULL, im?im->magic:0 );
	return im;
}

Bool save_asimage_to_file(const char *file2bsaved, ASImage *im,
	           const char *strtype,
			   const char *compress,
			   const char *opacity,
			   int delay, int replace)
{
	ASImageExportParams params ;

	memset( &params, 0x00, sizeof(params) );
	params.gif.flags = EXPORT_ALPHA ;
	if (strtype == NULL || !mystrcasecmp(strtype, "jpeg") || !mystrcasecmp(strtype, "jpg"))  {
		params.type = ASIT_Jpeg;
		params.jpeg.quality = (compress==NULL)?-1:100-atoi(compress);
		if( params.jpeg.quality > 100 )
			params.jpeg.quality = 100;
	} else if (!mystrcasecmp(strtype, "bitmap") || !mystrcasecmp(strtype, "bmp")) {
		params.type = ASIT_Bmp;
	} else if (!mystrcasecmp(strtype, "png")) {
		params.type = ASIT_Png;
		params.png.compression = (compress==NULL)?-1:atoi(compress)/10;
		if( params.png.compression > 9 )
			params.png.compression = 9;
	} else if (!mystrcasecmp(strtype, "xcf")) {
		params.type = ASIT_Xcf;
	} else if (!mystrcasecmp(strtype, "ppm")) {
		params.type = ASIT_Ppm;
	} else if (!mystrcasecmp(strtype, "pnm")) {
		params.type = ASIT_Pnm;
	} else if (!mystrcasecmp(strtype, "ico")) {
		params.type = ASIT_Ico;
	} else if (!mystrcasecmp(strtype, "cur")) {
		params.type = ASIT_Cur;
	} else if (!mystrcasecmp(strtype, "gif")) {
		params.type = ASIT_Gif;
		params.gif.flags |= EXPORT_APPEND ;
		params.gif.opaque_threshold = (opacity==NULL)?127:atoi(opacity) ;
		params.gif.dither = (compress==NULL)?3:atoi(compress)/17;
		if( params.gif.dither > 6 )
			params.gif.dither = 6;
		params.gif.animate_delay = delay ;
	} else if (!mystrcasecmp(strtype, "xpm")) {
		params.type = ASIT_Xpm;
		params.xpm.opaque_threshold = (opacity==NULL)?127:atoi(opacity) ;
		params.xpm.dither = (compress==NULL)?3:atoi(compress)/17;
		if( params.xpm.dither > 6 )
			params.xpm.dither = 6;
	} else if (!mystrcasecmp(strtype, "xbm")) {
		params.type = ASIT_Xbm;
	} else if (!mystrcasecmp(strtype, "tiff")) {
		params.type = ASIT_Tiff;
		params.tiff.compression_type = TIFF_COMPRESSION_NONE ;
		if( compress )
		{
			if( mystrcasecmp( compress, "deflate" ) == 0 )
				params.tiff.compression_type = TIFF_COMPRESSION_DEFLATE ;
			else if( mystrcasecmp( compress, "jpeg" ) == 0 )
				params.tiff.compression_type = TIFF_COMPRESSION_JPEG ;
			else if( mystrcasecmp( compress, "ojpeg" ) == 0 )
				params.tiff.compression_type = TIFF_COMPRESSION_OJPEG ;
			else if( mystrcasecmp( compress, "packbits" ) == 0 )
				params.tiff.compression_type = TIFF_COMPRESSION_PACKBITS ;
		}
	} else {
		show_error("File type not found.");
		return(0);
	}

	if( replace && file2bsaved )
		unlink( file2bsaved );

	return ASImage2file(im, NULL, file2bsaved, params.type, &params);

}

void show_asimage(ASVisual *asv, ASImage* im, Window w, long delay)
{
#ifndef X_DISPLAY_MISSING
	if ( im && w )
	{
		Pixmap p = asimage2pixmap(asv, w, im, NULL, False);
		struct timeval value;

		XSetWindowBackgroundPixmap( dpy, w, p );
		XClearWindow( dpy, w );
		XFlush( dpy );
		XFreePixmap( dpy, p );
		p = None ;
		value.tv_usec = delay % 10000;
		value.tv_sec = delay / 10000;
		PORTABLE_SELECT (1, 0, 0, 0, &value);
	}
#endif /* X_DISPLAY_MISSING */
}

/* Math expression parsing algorithm. */
double parse_math(const char* str, char** endptr, double size) {
	double total = 0;
	char op = '+';
	char minus = 0;
	const char* startptr = str;
	if( str == NULL ) 
		return 0 ;
	while (*str) {
		while (isspace((int)*str)) str++;
		if (!op) {
			if (*str == '+' || *str == '-' || *str == '*' || *str == '/') op = *str++;
			else if (*str == '-') { minus = 1; str++; }
			else if (*str == ')') { str++; break; }
			else break;
		} else {
			char* ptr;
			double num;
			if (*str == '(') num = parse_math(str + 1, &ptr, size);
                      else if (*str == '$') {
                              for (ptr = (char*)str + 1 ; *ptr && !isspace(*ptr) && *ptr != '+' && *ptr != '-' && *ptr != '*' && *ptr != '/' && *ptr != ')' ; ptr++);
                              num = asxml_var_nget((char*)str + 1, ptr - (str + 1));
                      }
			else num = strtod(str, &ptr);
			if (str != ptr) {
				if (*ptr == '%') num *= size / 100.0, ptr++;
				if (minus) num = -num;
				if (op == '+') total += num;
				else if (op == '-') total -= num;
				else if (op == '*') total *= num;
				else if (op == '/' && num) total /= num;
			} else break;
			str = ptr;
			op = '\0';
			minus = 0;
		}
	}
	if (endptr) *endptr = (char*)str;
	show_debug(__FILE__,"parse_math",__LINE__,"Parsed math [%s] with reference [%.2f] into number [%.2f].", startptr, size, total);
	return total;
}

typedef struct ASImageXMLState
{
	ASFlagType 		flags ;
 	ASVisual 		*asv;
	ASImageManager 	*imman ;
	ASFontManager 	*fontman ;
	
}ASImageXMLState;


ASImage *commit_xml_image_built( ASImageXMLState *state, char *id, ASImage *result )
{	
	if (state && id && result) 
	{
    	char* buf = NEW_ARRAY(char, strlen(id) + 1 + 6 + 1);
		show_progress("Storing image id [%s] with image manager %p .", id, state->imman);
        sprintf(buf, "%s.width", id);
        asxml_var_insert(buf, result->width);
        sprintf(buf, "%s.height", id);
        asxml_var_insert(buf, result->height);
        free(buf);
		if( result->imageman != NULL )
		{
			ASImage *tmp = clone_asimage(result, SCL_DO_ALL );
			safe_asimage_destroy(result );
			result = tmp ;
		}
		if( result )
		{
			if( !store_asimage( state->imman, result, id ) )
			{
				show_warning("Failed to store image id [%s].", id);
				safe_asimage_destroy(result );
				result = fetch_asimage( state->imman, id );
				/*show_warning("Old image with the name fetched as %p.", result);*/
			}else
			{
				/* normally generated image will be destroyed right away, so we need to
			 	* increase ref count, in order to preserve it for future uses : */
				dup_asimage( result );
			}
		}
	}
	return result;
}



/* Each tag is only allowed to return ONE image. */
ASImage*
build_image_from_xml( ASVisual *asv, ASImageManager *imman, ASFontManager *fontman, xml_elem_t* doc, xml_elem_t** rparm, ASFlagType flags, int verbose, Window display_win)
{
	xml_elem_t* ptr;
	char* id = NULL;
	ASImage* result = NULL;

	ASImageXMLState state ; 

	memset( &state, 0x00, sizeof(state));
	state.asv = asv ; 
	state.imman = imman ; 
	state.fontman = fontman ; 
/****** libAfterImage/asimagexml/img
 * NAME
 * img - load image from the file.
 * SYNOPSIS
 * <img id="new_img_id" src=filename/>
 * ATTRIBUTES
 * id     Optional.  Image will be given this name for future reference.
 * src    Required.  The filename (NOT URL) of the image file to load.
 * NOTES
 * The special image src "xroot:" will import the background image
 * of the root X window, if any.  No attempt will be made to offset this
 * image to fit the location of the resulting window, if one is displayed.
 ******/
	if (!strcmp(doc->tag, "img")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* src = NULL;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "src")) src = ptr->parm;
		}
		if (src && !strcmp(src, "xroot:")) {
			unsigned int width, height;
			Pixmap rp = GetRootPixmap(None);
			show_progress("Getting root pixmap.");
			if (rp) {
				get_drawable_size(rp, &width, &height);
				result = pixmap2asimage(asv, rp, 0, 0, width, height, 0xFFFFFFFF, False, 100);
			}
		} else if (src) {
			show_progress("Loading image [%s] using imman (%p) with search path \"%s\".", src, imman, imman?imman->search_path[0]:"");
#if 1
			result = get_asimage( imman, src, 0xFFFFFFFF, 100 );
#else
			result = file2ASImage(src, 0xFFFFFFFF, SCREEN_GAMMA, 100, NULL);
#endif

		}
		if (rparm) *rparm = parm; 
		else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/recall
 * NAME
 * recall - recall previously generated and named image by its id.
 * SYNOPSIS
 * <recall id="new_id" srcid="image_id">
 * ATTRIBUTES
 * id       Optional.  Image will be given this name for future reference.
 * srcid    Required.  An image ID defined with the "id" parameter for
 *          any previously created image.
 ******/
	if (!strcmp(doc->tag, "recall")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* srcid = NULL;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "srcid")) srcid = ptr->parm;
		}
		if (srcid) {
			show_progress("Recalling image id [%s] from imman %p.", srcid, imman);
			result = fetch_asimage(imman, srcid );
			if (!result)
				show_error("Image recall failed for id [%s].", srcid);
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}
/****** libAfterImage/asimagexml/release
 * NAME
 * release - release(destroy if possible) previously generated and named image by its id.
 * SYNOPSIS
 * <release srcid="image_id">
 * ATTRIBUTES
 * srcid    Required.  An image ID defined with the "id" parameter for
 *          any previously created image.
 ******/
	if (!strcmp(doc->tag, "release")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* srcid = NULL;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "srcid")) srcid = ptr->parm;
		}
		if (srcid) 
		{
			show_progress("Releasing image id [%s] from imman %p.", srcid, imman);
			release_asimage_by_name(imman, (char*)srcid );
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}
/****** libAfterImage/asimagexml/color
 * NAME
 * color - defines symbolic name for a color and set of variables.
 * SYNOPSIS
 * <color name="sym_name" domain="var_domain" argb=colorvalue/>
 * ATTRIBUTES
 * name   Symbolic name for the color value, to be used to refer to that color.
 * argb   8 characters hex definition of the color or other symbolic color name.
 * domain string to be used to prepend names of defined variables.
 * NOTES
 * In addition to defining symbolic name for the color this tag will define
 * 7 other variables : 	domain.sym_name.red, domain.sym_name.green, 
 * 					   	domain.sym_name.blue, domain.sym_name.alpha, 
 * 					  	domain.sym_name.hue, domain.sym_name.saturation,
 *                     	domain.sym_name.value
 ******/
	if (!strcmp(doc->tag, "color")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* name = NULL;
		const char* argb_text = NULL;
		const char* var_domain = NULL;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "name")) name = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "argb")) argb_text = ptr->parm;
			if (!strcmp(ptr->tag, "domain")) var_domain = ptr->parm;
		}
		if (name && argb_text) {
			ARGB32 argb = ARGB32_Black;
			if( parse_argb_color( argb_text, &argb ) != argb_text )
			{
				char *tmp;
				CARD32 hue16, sat16, val16 ;
				int vd_len = var_domain?strlen(var_domain):0 ;

				tmp = safemalloc( vd_len + 1+ strlen(name )+32+1 ) ;

				if( var_domain && var_domain[0] != '\0' )
				{
					if( var_domain[vd_len-1] != '.' )
					{
						sprintf( tmp, "%s.", var_domain );
						++vd_len ;
					}else
						strcpy( tmp, var_domain );
				}


#ifdef HAVE_AFTERBASE
	   			show_progress("defining synonim [%s] for color value #%8.8X.", name, argb);
	   			register_custom_color( name, argb );
#endif
				sprintf( tmp+vd_len, "%s.alpha", name );
				asxml_var_insert( tmp, ARGB32_ALPHA8(argb) );
				sprintf( tmp+vd_len, "%s.red", name );
				asxml_var_insert( tmp, ARGB32_RED8(argb) );
				sprintf( tmp+vd_len, "%s.green", name );
				asxml_var_insert( tmp, ARGB32_GREEN8(argb) );
				sprintf( tmp+vd_len, "%s.blue", name );
				asxml_var_insert( tmp, ARGB32_BLUE8(argb) );

				hue16 = rgb2hsv( ARGB32_RED16(argb), ARGB32_GREEN16(argb), ARGB32_BLUE16(argb), &sat16, &val16 );

				sprintf( tmp+vd_len, "%s.hue", name );
				asxml_var_insert( tmp, hue162degrees( hue16 ) );
				sprintf( tmp+vd_len, "%s.saturation", name );
				asxml_var_insert( tmp, val162percent( sat16 ) );
				sprintf( tmp+vd_len, "%s.value", name );
				asxml_var_insert( tmp, val162percent( val16 ) );
				free( tmp );
			}
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/text
 * NAME
 * text - render text string into new image, using specific font, size
 *        and texture.
 * SYNOPSIS
 * <text id="new_id" font="font" point="size" fgcolor="color"
 *       bgcolor="color" fgimage="image_id" bgimage="image_id"
 *       spacing="points">My Text Here</text>
 * ATTRIBUTES
 * id       Optional.  Image will be given this name for future reference.
 * font     Optional.  Default is "fixed".  Font to use for text.
 * point    Optional.  Default is 12.  Size of text in points.
 * fgcolor  Optional.  No default.  The text will be drawn in this color.
 * bgcolor  Optional.  No default.  The area behind the text will be drawn
 *          in this color.
 * fgimage  Optional.  No default.  The text will be textured by this image.
 * bgimage  Optional.  No default.  The area behind the text will be filled
 *          with this image.
 * spacing  Optional.  Default 0.  Extra pixels to place between each glyph.
 * type     Optional.  Default 0.  Valid values are from 0 to 7 and each 
 * 			represeend different 3d type.
 * NOTES
 * <text> without bgcolor, fgcolor, fgimage, or bgimage will NOT
 * produce visible output by itself.  See EXAMPLES below.
 ******/
	if (!strcmp(doc->tag, "text")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* text = NULL;
		const char* font_name = "fixed";
		const char* fgimage_str = NULL;
		const char* bgimage_str = NULL;
		const char* fgcolor_str = NULL;
		const char* bgcolor_str = NULL;
		ARGB32 fgcolor = ARGB32_White, bgcolor = ARGB32_Black;
		int point = 12, spacing = 0, type = AST_Plain;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "font")) font_name = ptr->parm;
			if (!strcmp(ptr->tag, "point")) point = strtol(ptr->parm, NULL, 0);
			if (!strcmp(ptr->tag, "spacing")) spacing = strtol(ptr->parm, NULL, 0);
			if (!strcmp(ptr->tag, "fgimage")) fgimage_str = ptr->parm;
			if (!strcmp(ptr->tag, "bgimage")) bgimage_str = ptr->parm;
			if (!strcmp(ptr->tag, "fgcolor")) fgcolor_str = ptr->parm;
			if (!strcmp(ptr->tag, "bgcolor")) bgcolor_str = ptr->parm;
			if (!strcmp(ptr->tag, "type")) type = strtol(ptr->parm, NULL, 0);
		}
		for (ptr = doc->child ; ptr && !result ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, cdata_str)) text = ptr->parm;
		}
		if (text && point > 0) {
			struct ASFont *font = NULL;
			show_progress("Rendering text [%s] with font [%s] size [%d].", text, font_name, point);
			if (fontman) font = get_asfont(fontman, font_name, 0, point, ASF_GuessWho);
			if (font != NULL) {
				set_asfont_glyph_spacing(font, spacing, 0);
				result = draw_text(text, font, type, 0);
				if (result && fgimage_str) {
					ASImage* fgimage = NULL;
					fgimage = get_asimage(imman, fgimage_str, 0xFFFFFFFF, 100 );
					show_progress("Using image [%s](%p) as foreground. Text size is %dx%d", fgimage_str, fgimage, result->width, result->height);
					if (fgimage) {
						ASImage *tmp = tile_asimage(asv, fgimage, 0, 0, result->width, result->height, 0, ASA_ASImage, 100, ASIMAGE_QUALITY_TOP);
						if( tmp )
						{
					   		release_asimage( fgimage );
							fgimage = tmp ;
						}
						move_asimage_channel(fgimage, IC_ALPHA, result, IC_ALPHA);
						safe_asimage_destroy(result);
						result = fgimage;
					}
				}
				if (result && fgcolor_str) {
					ASImage* fgimage = create_asimage(result->width, result->height, ASIMAGE_QUALITY_TOP);
					parse_argb_color(fgcolor_str, &fgcolor);
					fill_asimage(asv, fgimage, 0, 0, result->width, result->height, fgcolor);
					move_asimage_channel(fgimage, IC_ALPHA, result, IC_ALPHA);
					safe_asimage_destroy(result);
					result = fgimage;
				}
				if (result && (bgcolor_str || bgimage_str)) {
					ASImageLayer layers[2];
					init_image_layers(&(layers[0]), 2);
					if (bgimage_str) layers[0].im = fetch_asimage(imman, bgimage_str);
					if (bgcolor_str)
						if( parse_argb_color(bgcolor_str, &bgcolor) != bgcolor_str )
						{
							if( layers[0].im != NULL )
								layers[0].im->back_color = bgcolor ;
							else
								layers[0].solid_color = bgcolor ;
						}
					result->back_color = fgcolor ;
					layers[0].dst_x = 0;
					layers[0].dst_y = 0;
					layers[0].clip_width = result->width;
					layers[0].clip_height = result->height;
					layers[0].bevel = NULL;
					layers[1].im = result;
					layers[1].dst_x = 0;
					layers[1].dst_y = 0;
					layers[1].clip_width = result->width;
					layers[1].clip_height = result->height;
					result = merge_layers(asv, layers, 2, result->width, result->height, ASA_ASImage, 0, ASIMAGE_QUALITY_DEFAULT);
					safe_asimage_destroy( layers[0].im );
				}
			}
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/save
 * NAME
 * save - write generated/loaded image into the file of one of the
 *        supported types
 * SYNOPSIS
 * <save id="new_id" dst="filename" format="format" compress="value"
 *       opacity="value" replace="0|1" delay="mlsecs">
 * ATTRIBUTES
 * id       Optional.  Image will be given this name for future reference.
 * dst      Optional.  Name of file image will be saved to. If omitted
 *          image will be dumped into stdout - usefull for CGI apps.
 * format   Optional.  Ouput format of saved image.  Defaults to the
 *          extension of the "dst" parameter.  Valid values are the
 *          standard AS image file formats: xpm, jpg, png, gif, tiff.
 * compress Optional.  Compression level if supported by output file
 *          format. Valid values are in range of 0 - 100 and any of
 *          "deflate", "jpeg", "ojpeg", "packbits" for TIFF files.
 *          Note that JPEG and GIF will produce images with deteriorated
 *          quality when compress is greater then 0. For JPEG default is
 *          25, for PNG default is 6 and for GIF it is 0.
 * opacity  Optional. Level below which pixel is considered to be
 *          transparent, while saving image as XPM or GIF. Valid values
 *          are in range 0-255. Default is 127.
 * replace  Optional. Causes ascompose to delete file if the file with the
 *          same name already exists. Valid values are 0 and 1. Default
 *          is 1 - files are deleted before being saved. Disable this to
 *          get multimage animated gifs.
 * delay    Optional. Delay to be stored in GIF image. This could be used
 *          to create animated gifs. Note that you have to set replace="0"
 *          and then write several images into the GIF file with the same
 *          name.
 * NOTES
 * This tag applies to the first image contained within the tag.  Any
 * further images will be discarded.
 *******/
	if (!strcmp(doc->tag, "save")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* dst = NULL;
		const char* ext = NULL;
		const char* compress = NULL ;
		const char* opacity = NULL ;
		int delay = 0 ;
		int replace = 1;
		/*<save id="" dst="" format="" compression="" delay="" replace="" opacity=""> */
		int autoext = 0;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			else if (!strcmp(ptr->tag, "dst")) dst = ptr->parm;
			else if (!strcmp(ptr->tag, "format")) ext = ptr->parm;
			else if (!strncmp(ptr->tag, "compress", 8)) compress = ptr->parm;
			else if (!strcmp(ptr->tag, "opacity")) opacity = ptr->parm;
			else if (!strcmp(ptr->tag, "delay"))   delay = atoi(ptr->parm);
			else if (!strcmp(ptr->tag, "replace")) replace = atoi(ptr->parm);
		}
		if (dst && !ext) {
			ext = strrchr(dst, '.');
			if (ext) ext++;
			autoext = 1;
		}
		for (ptr = doc->child ; ptr && !result ; ptr = ptr->next) {
			result = build_image_from_xml(asv, imman, fontman, ptr, NULL, flags, verbose, display_win);
		}
		if ( autoext && ext )
			show_warning("No format given.  File extension [%s] used as format.", ext);
		show_progress("reSaving image to file [%s].", dst?dst:"stdout");
		if (result && get_flags( flags, ASIM_XML_ENABLE_SAVE) )
		{
			show_progress("Saving image to file [%s].", dst?dst:"stdout");
			if( !save_asimage_to_file(dst, result, ext, compress, opacity, delay, replace))
				show_error("Unable to save image into file [%s].", dst?dst:"stdout");
		}
		if (rparm) *rparm = parm;
		else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/bevel
 * NAME
 * bevel - draws solid bevel frame around the image.
 * SYNOPSIS
 * <bevel id="new_id" colors="color1 color2"
 *        border="left top right bottom" solid=0|1>
 * ATTRIBUTES
 * id       Optional.  Image will be given this name for future reference.
 * colors   Optional.  Whitespace-separated list of colors.  Exactly two
 *          colors are required.  Default is "#ffdddddd #ff555555".  The
 *          first color is the color of the upper and left edges, and the
 *          second is the color of the lower and right edges.
 * borders  Optional.  Whitespace-separated list of integer values.
 *          Default is "10 10 10 10".  The values represent the offsets
 *          toward the center of the image of each border: left, top,
 *          right, bottom.
 * solid    Optional - default is 1. If set to 0 will draw bevel gradually
 *          fading into the image.
 * NOTES
 * This tag applies to the first image contained within the tag.  Any
 * further images will be discarded.
 ******/
	if (!strcmp(doc->tag, "bevel")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		ASImage* imtmp = NULL;
		char* color_str = NULL;
		char* border_str = NULL;
		int solid = 1 ;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "colors")) color_str = ptr->parm;
			if (!strcmp(ptr->tag, "border")) border_str = ptr->parm;
			if (!strcmp(ptr->tag, "solid")) solid = atoi(ptr->parm);
		}
		for (ptr = doc->child ; ptr && !imtmp ; ptr = ptr->next) {
			imtmp = build_image_from_xml(asv, imman, fontman, ptr, NULL, flags, verbose, display_win);;
		}
		if (imtmp) {
			ASImageBevel bevel;
			ASImageLayer layer;
			if( solid )
				bevel.type = BEVEL_SOLID_INLINE;
			bevel.hi_color = 0xffdddddd;
			bevel.lo_color = 0xff555555;
			bevel.top_outline = 0;
			bevel.left_outline = 0;
			bevel.right_outline = 0;
			bevel.bottom_outline = 0;
			bevel.top_inline = 10;
			bevel.left_inline = 10;
			bevel.right_inline = 10;
			bevel.bottom_inline = 10;
			if (color_str) {
				char* p = color_str;
				while (isspace((int)*p)) p++;
				p = (char*)parse_argb_color(p, &bevel.hi_color);
				while (isspace((int)*p)) p++;
				parse_argb_color(p, &bevel.lo_color);
			}
			if (border_str) {
				char* p = (char*)border_str;
				bevel.left_inline = (unsigned short)parse_math(p, &p, imtmp->width);
				bevel.top_inline = (unsigned short)parse_math(p, &p, imtmp->height);
				bevel.right_inline = (unsigned short)parse_math(p, &p, imtmp->width);
				bevel.bottom_inline = (unsigned short)parse_math(p, &p, imtmp->height);
			}
			bevel.hihi_color = bevel.hi_color;
			bevel.hilo_color = bevel.hi_color;
			bevel.lolo_color = bevel.lo_color;
			show_progress("Generating bevel with offsets [%d %d %d %d] and colors [#%08x #%08x].", bevel.left_inline, bevel.top_inline, bevel.right_inline, bevel.bottom_inline, (unsigned int)bevel.hi_color, (unsigned int)bevel.lo_color);
			init_image_layers( &layer, 1 );
			layer.im = imtmp;
			layer.clip_width = imtmp->width;
			layer.clip_height = imtmp->height;
			layer.bevel = &bevel;
			result = merge_layers(asv, &layer, 1, imtmp->width, imtmp->height, ASA_ASImage, 0, ASIMAGE_QUALITY_DEFAULT);
			safe_asimage_destroy(imtmp);
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/gradient
 * NAME
 * gradient - render multipoint gradient.
 * SYNOPSIS
 * <gradient id="new_id" angle="degrees" width="pixels" height="pixels"
 *           colors ="color1 color2 color3 [...]"
 *           offsets="fraction1 fraction2 fraction3 [...]"/>
 * ATTRIBUTES
 * id       Optional.  Image will be given this name for future reference.
 * refid    Optional.  An image ID defined with the "id" parameter for
 *          any previously created image.  If set, percentages in "width"
 *          and "height" will be derived from the width and height of the
 *          refid image.
 * width    Required.  The gradient will have this width.
 * height   Required.  The gradient will have this height.
 * colors   Required.  Whitespace-separated list of colors.  At least two
 *          colors are required.  Each color in this list will be visited
 *          in turn, at the intervals given by the offsets attribute.
 * offsets  Optional.  Whitespace-separated list of floating point values
 *          ranging from 0.0 to 1.0.  The colors from the colors attribute
 *          are given these offsets, and the final gradient is rendered
 *          from the combination of the two.  If both colors and offsets
 *          are given but the number of colors and offsets do not match,
 *          the minimum of the two will be used, and the other will be
 *          truncated to match.  If offsets are not given, a smooth
 *          stepping from 0.0 to 1.0 will be used.
 * angle    Optional.  Given in degrees.  Default is 0.  This is the
 *          direction of the gradient.  Currently the only supported
 *          values are 0, 45, 90, 135, 180, 225, 270, 315.  0 means left
 *          to right, 90 means top to bottom, etc.
 *****/
	if (!strcmp(doc->tag, "gradient")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* refid = NULL;
		const char* width_str = NULL;
		const char* height_str = NULL;
		int width = 0, height = 0;
		double angle = 0;
		char* color_str = NULL;
		char* offset_str = NULL;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "width")) width_str = ptr->parm;
			if (!strcmp(ptr->tag, "height")) height_str = ptr->parm;
			if (!strcmp(ptr->tag, "angle")) angle = strtod(ptr->parm, NULL);
			if (!strcmp(ptr->tag, "colors")) color_str = ptr->parm;
			if (!strcmp(ptr->tag, "offsets")) offset_str = ptr->parm;
		}
		if (refid && width_str && height_str) {
			ASImage* refimg = fetch_asimage( imman, refid);
			if (refimg) {
				width = (int)parse_math(width_str, NULL, refimg->width);
				height = (int)parse_math(height_str, NULL, refimg->height);
			}
			safe_asimage_destroy(refimg);
		}
		if (!refid && width_str && height_str) {
			width = (int)parse_math(width_str, NULL, width);
			height = (int)parse_math(height_str, NULL, height);
		}
		if (width && height && color_str) {
			ASGradient gradient;
			int reverse = 0, npoints1 = 0, npoints2 = 0;
			char* p;
			angle = fmod(angle, 2 * PI);
			if (angle > 2 * PI * 15 / 16 || angle < 2 * PI * 1 / 16) {
				gradient.type = GRADIENT_Left2Right;
			} else if (angle < 2 * PI * 3 / 16) {
				gradient.type = GRADIENT_TopLeft2BottomRight;
			} else if (angle < 2 * PI * 5 / 16) {
				gradient.type = GRADIENT_Top2Bottom;
			} else if (angle < 2 * PI * 7 / 16) {
				gradient.type = GRADIENT_BottomLeft2TopRight; reverse = 1;
			} else if (angle < 2 * PI * 9 / 16) {
				gradient.type = GRADIENT_Left2Right; reverse = 1;
			} else if (angle < 2 * PI * 11 / 16) {
				gradient.type = GRADIENT_TopLeft2BottomRight; reverse = 1;
			} else if (angle < 2 * PI * 13 / 16) {
				gradient.type = GRADIENT_Top2Bottom; reverse = 1;
			} else {
				gradient.type = GRADIENT_BottomLeft2TopRight;
			}
			for (p = color_str ; isspace((int)*p) ; p++);
			for (npoints1 = 0 ; *p ; npoints1++) {
				if (*p) for ( ; *p && !isspace((int)*p) ; p++);
				for ( ; isspace((int)*p) ; p++);
			}
			if (offset_str) {
				for (p = offset_str ; isspace((int)*p) ; p++);
				for (npoints2 = 0 ; *p ; npoints2++) {
					if (*p) for ( ; *p && !isspace((int)*p) ; p++);
					for ( ; isspace((int)*p) ; p++);
				}
			}
			if (npoints1 > 1) {
				int i;
				if (offset_str && npoints1 > npoints2) npoints1 = npoints2;
				gradient.color = safecalloc(npoints1, sizeof(ARGB32));
				gradient.offset = NEW_ARRAY(double, npoints1);
				for (p = color_str ; isspace((int)*p) ; p++);
				for (npoints1 = 0 ; *p ; ) {
					char* pb = p, ch;
					if (*p) for ( ; *p && !isspace((int)*p) ; p++);
					for ( ; isspace((int)*p) ; p++);
					ch = *p; *p = '\0';
					if (parse_argb_color(pb, gradient.color + npoints1) != pb)
					{
						npoints1++;
					}else
						show_warning( "failed to parse color [%s] - defaulting to black", pb );
					*p = ch;
				}
				if (offset_str) {
					for (p = offset_str ; isspace((int)*p) ; p++);
					for (npoints2 = 0 ; *p ; ) {
						char* pb = p, ch;
						if (*p) for ( ; *p && !isspace((int)*p) ; p++);
						ch = *p; *p = '\0';
						gradient.offset[npoints2] = strtod(pb, &pb);
						if (pb == p) npoints2++;
						*p = ch;
						for ( ; isspace((int)*p) ; p++);
					}
				} else {
					for (npoints2 = 0 ; npoints2 < npoints1 ; npoints2++)
						gradient.offset[npoints2] = (double)npoints2 / (npoints1 - 1);
				}
				gradient.npoints = npoints1;
				if (npoints2 && gradient.npoints > npoints2)
					gradient.npoints = npoints2;
				if (reverse) {
					for (i = 0 ; i < gradient.npoints / 2 ; i++) {
						int i2 = gradient.npoints - 1 - i;
						ARGB32 c = gradient.color[i];
						double o = gradient.offset[i];
						gradient.color[i] = gradient.color[i2];
						gradient.color[i2] = c;
						gradient.offset[i] = gradient.offset[i2];
						gradient.offset[i2] = o;
					}
					for (i = 0 ; i < gradient.npoints ; i++) {
						gradient.offset[i] = 1.0 - gradient.offset[i];
					}
				}
				show_progress("Generating [%dx%d] gradient with angle [%f] and npoints [%d/%d].", width, height, angle, npoints1, npoints2);
				if (verbose > 1) {
					for (i = 0 ; i < gradient.npoints ; i++) {
						show_progress("  Point [%d] has color [#%08x] and offset [%f].", i, (unsigned int)gradient.color[i], gradient.offset[i]);
					}
				}
				result = make_gradient(asv, &gradient, width, height, SCL_DO_ALL, ASA_ASImage, 0, ASIMAGE_QUALITY_DEFAULT);
				if( gradient.color ) 
					free( gradient.color );
				if( gradient.offset )
					free( gradient.offset );
			}
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/mirror
 * NAME
 * mirror - create new image as mirror copy of an old one.
 * SYNOPSIS
 *  <mirror id="new_id" dir="direction">
 * ATTRIBUTES
 * id       Optional. Image will be given this name for future reference.
 * dir      Required. Possible values are "vertical" and "horizontal".
 *          The image will be flipped over the x-axis if dir is vertical,
 *          and flipped over the y-axis if dir is horizontal.
 * NOTES
 * This tag applies to the first image contained within the tag.  Any
 * further images will be discarded.
 ******/
	if (!strcmp(doc->tag, "mirror")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		ASImage* imtmp = NULL;
		int dir = 0;
		int static_im  = 0; 
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "dir")) dir = !mystrcasecmp(ptr->parm, "vertical");
			if (!strcmp(ptr->tag, "static")) static_im = atoi(ptr->parm);
		}
		for (ptr = doc->child ; ptr && !imtmp ; ptr = ptr->next) {
			imtmp = build_image_from_xml(asv, imman, fontman, ptr, NULL, flags, verbose, display_win);
		}
		if (imtmp) {
			result = mirror_asimage(asv, imtmp, 0, 0, imtmp->width, imtmp->height, dir,
									ASA_ASImage, 
									0, ASIMAGE_QUALITY_DEFAULT);
			safe_asimage_destroy(imtmp);
		}
		show_progress("Mirroring image [%sally].", dir ? "horizont" : "vertic");
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}
/****** libAfterImage/asimagexml/background
 * NAME
 * background - set image's background color.
 * SYNOPSIS
 *  <background id="new_id" color="color">
 * ATTRIBUTES
 * id       Optional. Image will be given this name for future reference.
 * color    Required. Color to be used for background - fills all the
 *          spaces in image with missing pixels.
 * NOTES
 * This tag applies to the first image contained within the tag.  Any
 * further images will be discarded.
 ******/
	if (!strcmp(doc->tag, "background")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		ASImage* imtmp = NULL;
		ARGB32 argb = ARGB32_Black;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "color")) parse_argb_color( ptr->parm, &argb );
		}
		for (ptr = doc->child ; ptr && !imtmp ; ptr = ptr->next) {
			imtmp = build_image_from_xml(asv, imman, fontman, ptr, NULL, flags, verbose, display_win);
		}
		if (imtmp) {
			result = clone_asimage( imtmp, SCL_DO_ALL );
			safe_asimage_destroy(imtmp);
			result->back_color = argb ;
		}
		show_progress( "Setting back_color for image %p to 0x%8.8X", result, argb );
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}
/****** libAfterImage/asimagexml/blur
 * NAME
 * blur - perform a gaussian blurr on an image.
 * SYNOPSIS
 * <blur id="new_id" horz="radius" vert="radius" channels="argb">
 * ATTRIBUTES
 * id       Optional. Image will be given this name for future reference.
 * horz     Optional. Horizontal radius of the blur in pixels.
 * vert     Optional. Vertical radius of the blur in pixels.
 * channels Optional. Applys blur only on listed color channels:
 *                       a - alpha,
 *                       r - red,
 *                       g - green,
 *                       b - blue
 * NOTES
 * This tag applies to the first image contained within the tag.  Any
 * further images will be discarded.
 ******/
	if (!strcmp(doc->tag, "blur")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		ASImage* imtmp = NULL;
		int horz = 0, vert = 0;
        int filter = SCL_DO_ALL;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
            else if (!strcmp(ptr->tag, "horz")) horz = atoi(ptr->parm);
            else if (!strcmp(ptr->tag, "vert")) vert = atoi(ptr->parm);
            else if (!strcmp(ptr->tag, "channels"))
            {
                int i = 0 ;
                char *str = &(ptr->parm[0]) ;
                filter = 0 ;
                while( str[i] != '\0' )
                {
                    if( str[i] == 'a' )
                        filter |= SCL_DO_ALPHA ;
                    else if( str[i] == 'r' )
                        filter |= SCL_DO_RED ;
                    else if( str[i] == 'g' )
                        filter |= SCL_DO_GREEN ;
                    else if( str[i] == 'b' )
                        filter |= SCL_DO_BLUE ;
                    ++i ;
                }
            }
		}
		for (ptr = doc->child ; ptr && !imtmp ; ptr = ptr->next) {
			imtmp = build_image_from_xml(asv, imman, fontman, ptr, NULL, flags, verbose, display_win);
		}
		if (imtmp) {
            result = blur_asimage_gauss(asv, imtmp, horz, vert, filter, ASA_ASImage, 0, ASIMAGE_QUALITY_DEFAULT);
			safe_asimage_destroy(imtmp);
		}
		show_progress("Blurring image.");
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/rotate
 * NAME
 * rotate - rotate an image in 90 degree increments (flip).
 * SYNOPSIS
 *  <rotate id="new_id" angle="degrees">
 * ATTRIBUTES
 * id       Optional. Image will be given this name for future reference.
 * angle    Required.  Given in degrees.  Possible values are currently
 *          "90", "180", and "270".  Rotates the image through the given
 *          angle.
 * NOTES
 * This tag applies to the first image contained within the tag.  Any
 * further images will be discarded.
 ******/
	if (!strcmp(doc->tag, "rotate")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		ASImage* imtmp = NULL;
		double angle = 0;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "angle")) angle = strtod(ptr->parm, NULL);
		}
		for (ptr = doc->child ; ptr && !imtmp ; ptr = ptr->next) {
			imtmp = build_image_from_xml(asv, imman, fontman, ptr, NULL, flags, verbose, display_win);;
		}
		if (imtmp) {
			int dir = 0;
			angle = fmod(angle, 2 * PI);
			if (angle > 2 * PI * 7 / 8 || angle < 2 * PI * 1 / 8) {
				dir = 0;
			} else if (angle < 2 * PI * 3 / 8) {
				dir = FLIP_VERTICAL;
			} else if (angle < 2 * PI * 5 / 8) {
				dir = FLIP_UPSIDEDOWN;
			} else {
				dir = FLIP_VERTICAL | FLIP_UPSIDEDOWN;
			}
			if (dir) {
				int width = imtmp->width ;
				int height = imtmp->height ;
				if( get_flags(dir, FLIP_VERTICAL))
				{
					width = imtmp->height ;
					height = imtmp->width ;	
				}	 
				result = flip_asimage(asv, imtmp, 0, 0, width, height, dir, ASA_ASImage, 0, ASIMAGE_QUALITY_DEFAULT);
				safe_asimage_destroy(imtmp);
				show_progress("Rotating image [%f degrees].", angle);
			} else {
				result = imtmp;
			}
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/scale
 * NAME
 * scale - scale image to arbitrary size
 * SYNOPSIS
 * <scale id="new_id" ref_id="other_imag" width="pixels" height="pixels">
 * ATTRIBUTES
 * id       Optional. Image will be given this name for future reference.
 * refid    Optional.  An image ID defined with the "id" parameter for
 *          any previously created image.  If set, percentages in "width"
 *          and "height" will be derived from the width and height of the
 *          refid image.
 * width    Required.  The image will be scaled to this width.
 * height   Required.  The image will be scaled to this height.
 * NOTES
 * This tag applies to the first image contained within the tag.  Any
 * further images will be discarded.
 * If you want to keep image proportions while scaling - use "proportional"
 * instead of specific size for particular dimention.
 ******/
	if (!strcmp(doc->tag, "scale")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* refid = NULL;
		const char* width_str = NULL;
		const char* height_str = NULL;
		ASImage* imtmp = NULL;
		int width = 0, height = 0;
		int static_im = 0;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "refid")) refid = ptr->parm;
			if (!strcmp(ptr->tag, "width")) width_str = ptr->parm;
			if (!strcmp(ptr->tag, "height")) height_str = ptr->parm;
			if (!strcmp(ptr->tag, "static")) static_im = atoi(ptr->parm);
		}
		if (width_str && height_str) 
		{
			int width_ref = 1;
			int height_ref = 1;
			for (ptr = doc->child ; ptr && !imtmp ; ptr = ptr->next) 
				imtmp = build_image_from_xml(asv, imman, fontman, ptr, NULL, flags, verbose, display_win);
			if( imtmp ) 
			{	
				width_ref = width = imtmp->width ;
				height_ref = height = imtmp->height ;
			}
			if (refid ) 
			{
				ASImage* refimg = fetch_asimage(imman, refid );
				if (refimg) 
				{
					width_ref = refimg->width;
					height_ref = refimg->height;
					release_asimage( refimg );
				}
			}
			if( width_str[0] == '$' || isdigit( (int)width_str[0] ) )
				width = (int)parse_math(width_str, NULL, width);
			if( height_str[0] == '$' || isdigit( (int)height_str[0] ) )
				height = (int)parse_math(height_str, NULL, height);
			if( mystrcasecmp(width_str,"proportional") == 0 )
				width = (width_ref * height) / height_ref ;
			else if( mystrcasecmp(height_str,"proportional") == 0 )
				height = (height_ref * width) / width_ref ;
		}
		if (imtmp && width > 0 && height > 0 ) {
			show_progress("Scaling image to [%dx%d].", width, height);
			result = scale_asimage( asv, imtmp, width, height, 
									ASA_ASImage, 100, ASIMAGE_QUALITY_DEFAULT);
			safe_asimage_destroy(imtmp);
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/crop
 * NAME
 * crop - crop image to arbitrary area within it.
 * SYNOPSIS
 *  <crop id="new_id" refid="other_image" srcx="pixels" srcy="pixels"
 *        width="pixels" height="pixels" tint="color">
 * ATTRIBUTES
 * id       Optional. Image will be given this name for future reference.
 * refid    Optional. An image ID defined with the "id" parameter for
 *          any previously created image.  If set, percentages in "width"
 *          and "height" will be derived from the width and height of the
 *          refid image.
 * srcx     Optional. Default is "0". Skip this many pixels from the left.
 * srcy     Optional. Default is "0". Skip this many pixels from the top.
 * width    Optional. Default is "100%".  Keep this many pixels wide.
 * height   Optional. Default is "100%".  Keep this many pixels tall.
 * tint     Optional. Additionally tint an image to specified color.
 *          Tinting can both lighten and darken an image. Tinting color
 *          0 or #7f7f7f7f yeilds no tinting. Tinting can be performed on
 *          any channel, including alpha channel.
 * NOTES
 * This tag applies to the first image contained within the tag.  Any
 * further images will be discarded.
 ******/
	if (!strcmp(doc->tag, "crop")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* refid = NULL;
		const char* srcx_str = NULL;
		const char* srcy_str = NULL;
		const char* width_str = NULL;
		const char* height_str = NULL;
		ARGB32 tint = 0 ;
		int width = 0, height = 0, srcx = 0, srcy = 0;
		ASImage* imtmp = NULL;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "refid")) refid = ptr->parm;
			if (!strcmp(ptr->tag, "srcx")) srcx_str = ptr->parm;
			if (!strcmp(ptr->tag, "srcy")) srcy_str = ptr->parm;
			if (!strcmp(ptr->tag, "width")) width_str = ptr->parm;
			if (!strcmp(ptr->tag, "height")) height_str = ptr->parm;
			if (!strcmp(ptr->tag, "tint")) parse_argb_color(ptr->parm, &tint);
		}
		for (ptr = doc->child ; ptr && !imtmp ; ptr = ptr->next) {
			imtmp = build_image_from_xml(asv, imman, fontman, ptr, NULL, flags, verbose, display_win);
		}
		if (imtmp) {
			width = imtmp->width;
			height = imtmp->height;
			if (refid) {
				ASImage* refimg = fetch_asimage(imman, refid);
				if (refimg) {
					width = refimg->width;
					height = refimg->height;
				}
				safe_asimage_destroy( refimg );
			}
			if (srcx_str) srcx = (int)parse_math(srcx_str, NULL, width);
			if (srcy_str) srcy = (int)parse_math(srcy_str, NULL, height);
			if (width_str) width = (int)parse_math(width_str, NULL, width);
			if (height_str) height = (int)parse_math(height_str, NULL, height);
			if (width > (int)imtmp->width) width = imtmp->width;
			if (height > (int)imtmp->height) height = imtmp->height;
			if (width > 0 && height > 0) {
				result = tile_asimage(asv, imtmp, srcx, srcy, width, height, tint, ASA_ASImage, 100, ASIMAGE_QUALITY_TOP);
				safe_asimage_destroy(imtmp);
			}
			show_progress("Cropping image to [%dx%d].", width, height);
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}
/****** libAfterImage/asimagexml/tile
 * NAME
 * tile - tile an image to specified area.
 * SYNOPSIS
 *  <tile id="new_id" refid="other_image" width="pixels" height="pixels"
 *        x_origin="pixels" y_origin="pixels" tint="color" complement=0|1>
 * ATTRIBUTES
 * id       Optional. Image will be given this name for future reference.
 * refid    Optional. An image ID defined with the "id" parameter for
 *          any previously created image.  If set, percentages in "width"
 *          and "height" will be derived from the width and height of the
 *          refid image.
 * width    Optional. Default is "100%". The image will be tiled to this
 *          width.
 * height   Optional. Default is "100%". The image will be tiled to this
 *          height.
 * x_origin Optional. Horizontal position on infinite surface, covered
 *          with tiles of the image, from which to cut out resulting
 *          image.
 * y_origin Optional. Vertical position on infinite surface, covered
 *          with tiles of the image, from which to cut out resulting
 *          image.
 * tint     Optional. Additionally tint an image to specified color.
 *          Tinting can both lighten and darken an image. Tinting color
 *          0 or #7f7f7f7f yields no tinting. Tinting can be performed
 *          on any channel, including alpha channel.
 * complement Optional. Will use color that is the complement to tint color
 *          for the tinting, if set to 1. Default is 0.
 *
 * NOTES
 * This tag applies to the first image contained within the tag.  Any
 * further images will be discarded.
 ******/
	if (!strcmp(doc->tag, "tile")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* refid = NULL;
		const char* xorig_str = NULL;
		const char* yorig_str = NULL;
		const char* width_str = "100%";
		const char* height_str = "100%";
		int width = 0, height = 0, xorig = 0, yorig = 0;
		ARGB32 tint = 0 ;
		ASImage* imtmp = NULL;
		char *complement_str = NULL ;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			else if (!strcmp(ptr->tag, "refid")) refid = ptr->parm;
			else if (!strcmp(ptr->tag, "x_origin")) xorig_str = ptr->parm;
			else if (!strcmp(ptr->tag, "y_origin")) yorig_str = ptr->parm;
			else if (!strcmp(ptr->tag, "width")) width_str = ptr->parm;
			else if (!strcmp(ptr->tag, "height")) height_str = ptr->parm;
			else if (!strcmp(ptr->tag, "tint")) parse_argb_color(ptr->parm, &tint);
			else if (!strcmp(ptr->tag, "complement")) complement_str = ptr->parm;
		}
		for (ptr = doc->child ; ptr && !imtmp ; ptr = ptr->next) {
			imtmp = build_image_from_xml(asv, imman, fontman, ptr, NULL, flags, verbose, display_win);
		}
		if (imtmp) {
			width = imtmp->width;
			height = imtmp->height;
			if (refid) {
				ASImage* refimg = fetch_asimage(imman, refid);;
				if (refimg) {
					width = refimg->width;
					height = refimg->height;
				}
				safe_asimage_destroy( refimg );
			}
			if (width_str) width = (int)parse_math(width_str, NULL, width);
			if (height_str) height = (int)parse_math(height_str, NULL, height);
			if (xorig_str) xorig = (int)parse_math(xorig_str, NULL, width);
			if (yorig_str) yorig = (int)parse_math(yorig_str, NULL, height);
			if (width > 0 && height > 0)
			{
				if( complement_str )
				{
					register char *ptr = complement_str ;
					CARD32 a = ARGB32_ALPHA8(tint),
						   r = ARGB32_RED8(tint),
						   g = ARGB32_GREEN8(tint),
						   b = ARGB32_BLUE8(tint) ;
					while( *ptr )
					{
						if( *ptr == 'a' ) 		a = ~a ;
						else if( *ptr == 'r' ) 	r = ~r ;
						else if( *ptr == 'g' ) 	g = ~g ;
						else if( *ptr == 'b' ) 	b = ~b ;
						++ptr ;
					}

					tint = MAKE_ARGB32(a, r, g, b );
				}
				result = tile_asimage(asv, imtmp, xorig, yorig, width, height, tint, ASA_ASImage, 100, ASIMAGE_QUALITY_TOP);
				safe_asimage_destroy(imtmp);
			}
			show_progress("Tiling image to [%dx%d].", width, height);
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}
/****** libAfterImage/asimagexml/hsv
 * NAME
 * hsv - adjust Hue, Saturation and/or Value of an image and optionally
 * tile an image to arbitrary area.
 * SYNOPSIS
 * <hsv id="new_id" refid="other_image"
 *      x_origin="pixels" y_origin="pixels" width="pixels" height="pixels"
 *      affected_hue="degrees|color" affected_radius="degrees"
 *      hue_offset="degrees" saturation_offset="value"
 *      value_offset="value">
 * ATTRIBUTES
 * id       Optional. Image will be given this name for future reference.
 * refid    Optional. An image ID defined with the "id" parameter for
 *          any previously created image.  If set, percentages in "width"
 *          and "height" will be derived from the width and height of the
 *          refid image.
 * width    Optional. Default is "100%". The image will be tiled to this
 *          width.
 * height   Optional. Default is "100%". The image will be tiled to this
 *          height.
 * x_origin Optional. Horizontal position on infinite surface, covered
 *          with tiles of the image, from which to cut out resulting
 *          image.
 * y_origin Optional. Vertical position on infinite surface, covered
 *          with tiles of the image, from which to cut out resulting
 *          image.
 * affected_hue    Optional. Limits effects to the renage of hues around
 *          this hue. If numeric value is specified - it is treated as
 *          degrees on 360 degree circle, with :
 *              red = 0,
 *              yellow = 60,
 *              green = 120,
 *              cyan = 180,
 *              blue = 240,
 *              magenta = 300.
 *          If colorname or value preceded with # is specified here - it
 *          will be treated as RGB color and converted into hue
 *          automagically.
 * affected_radius
 *          Optional. Value in degrees to be used in order to
 *          calculate the range of affected hues. Range is determined by
 *          substracting and adding this value from/to affected_hue.
 * hue_offset
 *          Optional. Value by which to adjust the hue.
 * saturation_offset
 *          Optional. Value by which to adjust the saturation.
 * value_offset
 *          Optional. Value by which to adjust the value.
 * NOTES
 * One of the Offsets must be not 0, in order for operation to be
 * performed.
 *
 * This tag applies to the first image contained within the tag.  Any
 * further images will be discarded.
 ******/
	if (!strcmp(doc->tag, "hsv")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* refid = NULL;
		const char* xorig_str = NULL;
		const char* yorig_str = NULL;
		const char* width_str = "100%";
		const char* height_str = "100%";
		int affected_hue = 0, affected_radius = 360 ;
		int hue_offset = 0, saturation_offset = 0, value_offset = 0 ;
		int width = 0, height = 0, xorig = 0, yorig = 0;
		ASImage* imtmp = NULL;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			else if (!strcmp(ptr->tag, "refid")) refid = ptr->parm;
			else if (!strcmp(ptr->tag, "x_origin")) xorig_str = ptr->parm;
			else if (!strcmp(ptr->tag, "y_origin")) yorig_str = ptr->parm;
			else if (!strcmp(ptr->tag, "width")) width_str = ptr->parm;
			else if (!strcmp(ptr->tag, "height")) height_str = ptr->parm;
			else if (!strcmp(ptr->tag, "affected_hue"))
			{
				if( isdigit( (int)ptr->parm[0] ) )
					affected_hue = (int)parse_math(ptr->parm, NULL, 360);
				else
				{
					ARGB32 color = 0;
					if( parse_argb_color( ptr->parm, &color ) != ptr->parm )
					{
						affected_hue = rgb2hue( ARGB32_RED16(color),
												ARGB32_GREEN16(color),
												ARGB32_BLUE16(color));
  					    affected_hue = hue162degrees( affected_hue );
					}
				}
			}
			else if (!strcmp(ptr->tag, "affected_radius")) 	affected_radius = (int)parse_math(ptr->parm, NULL, 360);
			else if (!strcmp(ptr->tag, "hue_offset")) 		hue_offset = (int)parse_math(ptr->parm, NULL, 360);
			else if (!strcmp(ptr->tag, "saturation_offset"))saturation_offset = (int)parse_math(ptr->parm, NULL, 100);
			else if (!strcmp(ptr->tag, "value_offset")) 	value_offset = (int)parse_math(ptr->parm, NULL, 100);
		}
		if( hue_offset == -1 && saturation_offset == -1 ) 
		{
			hue_offset = 0 ; 
			saturation_offset = -99 ;
		}
		for (ptr = doc->child ; ptr && !imtmp ; ptr = ptr->next) {
			imtmp = build_image_from_xml(asv, imman, fontman, ptr, NULL, flags, verbose, display_win);
		}
		if (imtmp) {
			width = imtmp->width;
			height = imtmp->height;
			if (refid) {
				ASImage* refimg = fetch_asimage(imman, refid);;
				if (refimg) {
					width = refimg->width;
					height = refimg->height;
				}
				safe_asimage_destroy( refimg );
			}
			if (width_str) width = (int)parse_math(width_str, NULL, width);
			if (height_str) height = (int)parse_math(height_str, NULL, height);
			if (xorig_str) xorig = (int)parse_math(xorig_str, NULL, width);
			if (yorig_str) yorig = (int)parse_math(yorig_str, NULL, height);
			if (width > 0 && height > 0 &&
				(hue_offset!=0 || saturation_offset != 0 || value_offset != 0 )) {
				result = adjust_asimage_hsv(asv, imtmp, xorig, yorig, width, height,
				                            affected_hue, affected_radius,
											hue_offset, saturation_offset, value_offset,
				                            ASA_ASImage, 100, ASIMAGE_QUALITY_TOP);
				safe_asimage_destroy(imtmp);
			}
			show_progress("adjusting HSV of the image by [%d,%d,%d] affected hues are %+d-%+d.result = %p", hue_offset, saturation_offset, value_offset, affected_hue-affected_radius, affected_hue+affected_radius, result);
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/pad
 * NAME
 * pad - pad an image with solid color rectangles.
 * SYNOPSIS
 * <pad id="new_id" refid="other_image" left="pixels" top="pixels"
 *      right="pixels" bottom="pixels" color="color">
 * ATTRIBUTES
 * id       Optional. Image will be given this name for future reference.
 * refid    Optional. An image ID defined with the "id" parameter for
 *          any previously created image.  If set, percentages in "pixel"
 *          pad values will be derived from the width and height of the
 *          refid image.
 * left     Optional. Size to add to the left of the image.
 * top      Optional. Size to add to the top of the image.
 * right    Optional. Size to add to the right of the image.
 * bottom   Optional. Size to add to the bottom of the image.
 * color    Optional. Color value to fill added areas with. It could be
 *          transparent of course. Default is #FF000000 - totally black.
 * NOTES
 * This tag applies to the first image contained within the tag.  Any
 * further images will be discarded.
 ******/
	if (!strcmp(doc->tag, "pad")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* refid = NULL;
		const char* left_str = "0";
		const char* top_str  = "0";
		const char* right_str  = "0";
		const char* bottom_str  = "0";
		ARGB32 color  = ARGB32_Black;
		int left = 0, top = 0, right = 0, bottom = 0;
		ASImage* imtmp = NULL;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			else if (!strcmp(ptr->tag, "refid"))  refid = ptr->parm;
			else if (!strcmp(ptr->tag, "left"))   left_str = ptr->parm;
			else if (!strcmp(ptr->tag, "top"))    top_str = ptr->parm;
			else if (!strcmp(ptr->tag, "right"))  right_str = ptr->parm;
			else if (!strcmp(ptr->tag, "bottom")) bottom_str = ptr->parm;
			else if (!strcmp(ptr->tag, "color"))  parse_argb_color(ptr->parm, &color);
		}
		for (ptr = doc->child ; ptr && !imtmp ; ptr = ptr->next) {
			imtmp = build_image_from_xml(asv, imman, fontman, ptr, NULL, flags, verbose, display_win);
		}
		if (imtmp) {
			int width = imtmp->width;
			int height = imtmp->height;
			if (refid) {
				ASImage* refimg = fetch_asimage(imman, refid);
				if (refimg) {
					width = refimg->width;
					height = refimg->height;
				}
				safe_asimage_destroy( refimg );
			}
			if (left_str) left = (int)parse_math(left_str, NULL, width);
			if (top_str)  top = (int)parse_math(top_str, NULL, height);
			if (right_str) right = (int)parse_math(right_str, NULL, width);
			if (bottom_str)  bottom = (int)parse_math(bottom_str, NULL, height);
			if (left > 0 || top > 0 || right > 0 || bottom > 0 )
			{
				result = pad_asimage(asv, imtmp, left, top, width+left+right, height+top+bottom,
					                 color, ASA_ASImage, 100, ASIMAGE_QUALITY_DEFAULT);
				safe_asimage_destroy(imtmp);
			}
			show_progress("Padding image to [%dx%d%+d%+d].", width+left+right, height+top+bottom, left, top);
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/solid
 * NAME
 * solid - generate image of specified size and fill it with solid color.
 * SYNOPSIS
 * <solid id="new_id" color="color" opacity="opacity" 
 * 	width="pixels" height="pixels"/>
 * ATTRIBUTES
 * id       Optional. Image will be given this name for future reference.
 * color    Optional.  Default is "#ffffffff".  An image will be created
 *          and filled with this color.
 * width    Required.  The image will have this width.
 * height   Required.  The image will have this height.
 * opacity  Optional. Default is 100. Values from 0 to 100 represent the
 *          opacity of resulting image with 100 being completely opaque.
 * 		    Effectively overrides alpha component of the color setting.
 ******/
	if (!strcmp(doc->tag, "solid")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* refid = NULL;
		const char* width_str = NULL;
		const char* height_str = NULL;
		int width = 0, height = 0;
		Bool opacity_set = False ;
		int opacity = 100 ;
		ARGB32 color = ARGB32_White;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "refid")) refid = ptr->parm;
			if (!strcmp(ptr->tag, "color")) parse_argb_color(ptr->parm, &color);
			if (!strcmp(ptr->tag, "width")) width_str = ptr->parm;
			if (!strcmp(ptr->tag, "height")) height_str = ptr->parm;
			if (!strcmp(ptr->tag, "opacity")) { opacity = atol(ptr->parm); opacity_set = True ; }
		}
		if (refid && width_str && height_str) {
			ASImage* refimg = fetch_asimage(imman, refid);
			if (refimg) {
				width = (int)parse_math(width_str, NULL, refimg->width);
				height = (int)parse_math(height_str, NULL, refimg->height);
			}
			safe_asimage_destroy( refimg );
		}
		if (!refid && width_str && height_str) {
			width = (int)parse_math(width_str, NULL, 0);
			height = (int)parse_math(height_str, NULL, 0);
		}
		if (width && height) {
			CARD32 a, r, g, b ;
			result = create_asimage(width, height, ASIMAGE_QUALITY_TOP);
			if( opacity < 0 ) opacity = 0 ;
			else if( opacity > 100 )  opacity = 100 ;
			a = opacity_set? (0x000000FF * (CARD32)opacity)/100: ARGB32_ALPHA8(color);
			r = ARGB32_RED8(color);
			g = ARGB32_GREEN8(color);
			b = ARGB32_BLUE8(color);
			color = MAKE_ARGB32(a,r,g,b);
			if (result) fill_asimage(asv, result, 0, 0, width, height, color);
			show_progress("Creating solid color [#%08x] image [%dx%d].", (unsigned int)color, width, height);
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}

/****** libAfterImage/asimagexml/composite
 * NAME
 * composite - superimpose arbitrary number of images on top of each
 * other.
 * SYNOPSIS
 * <composite id="new_id" op="op_desc"
 *            keep-transparency="0|1" merge="0|1">
 * ATTRIBUTES
 * id       Optional. Image will be given this name for future reference.
 * op       Optional. Default is "alphablend". The compositing operation.
 *          Valid values are the standard AS blending ops: add, alphablend,
 *          allanon, colorize, darken, diff, dissipate, hue, lighten,
 *          overlay, saturate, screen, sub, tint, value.
 * merge    Optional. Default is "expand". Valid values are "clip" and
 *          "expand". Determines whether final image will be expanded to
 *          the maximum size of the layers, or clipped to the bottom
 *          layer.
 * keep-transparency
 *          Optional. Default is "0". Valid values are "0" and "1". If
 *          set to "1", the transparency of the bottom layer will be
 *          kept for the final image.
 * NOTES
 * All images surrounded by this tag will be composited with the given op.
 *
 * ATTRIBUTES
 *  All tags surrounded by this tag may have some of the common attributes
 *  in addition to their normal ones.  Under no circumstances is there a 
 *  conflict with the normal child attributes:
 * 
 * crefid   Optional. An image ID defined with the "id" parameter for
 *          any previously created image. If set, percentages in "x"
 *          and "y" will be derived from the width and height of the
 *          crefid image.
 * x        Optional. Default is 0. Pixel coordinate of left edge.
 * y        Optional. Default is 0. Pixel coordinate of top edge.
 * align    Optional. Alternative to x - allowed values are right, center
 *          and left.
 * valign   Optional. Alternative to y - allowed values are top, middle
 *          and bottom.
 * clip_x   Optional. Default is 0. X Offset on infinite surface tiled
 *          with this image, from which to cut portion of an image to be
 *          used in composition.
 * clip_y   Optional. Default is 0. Y Offset on infinite surface tiled
 *          with this image, from which to cut portion of an image to be
 *          used in composition.
 * clip_width
 *          Optional. Default is image width. Tile image to this width
 *          prior to superimposition.
 * clip_height
 *          Optional. Default is image height. Tile image to this height
 *          prior to superimposition.
 * tile     Optional. Default is 0. If set will cause image to be tiled
 *          across entire composition, unless overridden by clip_width or
 *          clip_height.
 * tint     Optional. Additionally tint an image to specified color.
 *          Tinting can both lighten and darken an image. Tinting color
 *          0 or #7f7f7f7f yields no tinting. Tinting can be performed
 *          on any channel, including alpha channel.
 * SEE ALSO
 * libAfterImage
 ******/
	if (!strcmp(doc->tag, "composite")) {
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* pop = "alphablend";
		int keep_trans = 0;
		int merge = 0;
		int num;
		int static_im = 0 ;
		for (ptr = parm ; ptr ; ptr = ptr->next) {
			if (!strcmp(ptr->tag, "id")) id = strdup(ptr->parm);
			if (!strcmp(ptr->tag, "op")) pop = ptr->parm;
			if (!strcmp(ptr->tag, "keep-transparency")) keep_trans = strtol(ptr->parm, NULL, 0);
			if (!strcmp(ptr->tag, "merge") && !mystrcasecmp(ptr->parm, "clip")) merge = 1;
			if (!strcmp(ptr->tag, "static")) static_im = atoi(ptr->parm);
		}
		/* Find out how many subimages we have. */
		num = 0;
		for (ptr = doc->child ; ptr ; ptr = ptr->next) {
			if (strcmp(ptr->tag, cdata_str)) num++;
		}
		if (num) {
			int width = 0, height = 0;
			ASImageLayer *layers;
#define  ASXML_ALIGN_LEFT 	(0x01<<0)
#define  ASXML_ALIGN_RIGHT 	(0x01<<1)
#define  ASXML_ALIGN_TOP    (0x01<<2)
#define  ASXML_ALIGN_BOTTOM (0x01<<3)
			int *align ;
			int i ;

			/* Build the layers first. */
			layers = create_image_layers( num );
			align = safecalloc( num, sizeof(int));
			for (num = 0, ptr = doc->child ; ptr ; ptr = ptr->next) 
			{
				int x = 0, y = 0;
				int clip_x = 0, clip_y = 0;
				int clip_width = 0, clip_height = 0;
				ARGB32 tint = 0;
				Bool tile = False ;
				xml_elem_t* sparm = NULL;
				if (!strcmp(ptr->tag, cdata_str)) continue;
				if( (layers[num].im = build_image_from_xml(asv, imman, fontman, ptr, &sparm, flags, verbose, display_win)) != NULL )
				{
					clip_width = layers[num].im->width;
					clip_height = layers[num].im->height;
				}
				if (sparm) 
				{
					xml_elem_t* tmp;
					const char* x_str = NULL;
					const char* y_str = NULL;
					const char* clip_x_str = NULL;
					const char* clip_y_str = NULL;
					const char* clip_width_str = NULL;
					const char* clip_height_str = NULL;
					const char* refid = NULL;
					for (tmp = sparm ; tmp ; tmp = tmp->next) {
						if (!strcmp(tmp->tag, "crefid")) refid = tmp->parm;
						else if (!strcmp(tmp->tag, "x")) x_str = tmp->parm;
						else if (!strcmp(tmp->tag, "y")) y_str = tmp->parm;
						else if (!strcmp(tmp->tag, "clip_x")) clip_x_str = tmp->parm;
						else if (!strcmp(tmp->tag, "clip_y")) clip_y_str = tmp->parm;
						else if (!strcmp(tmp->tag, "clip_width")) clip_width_str = tmp->parm;
						else if (!strcmp(tmp->tag, "clip_height")) clip_height_str = tmp->parm;
						else if (!strcmp(tmp->tag, "tint")) parse_argb_color(tmp->parm, &tint);
						else if (!strcmp(tmp->tag, "tile")) tile = True;
						else if (!strcmp(tmp->tag, "align"))
						{
							if (!strcmp(tmp->parm, "left"))set_flags( align[num], ASXML_ALIGN_LEFT);
							else if (!strcmp(tmp->parm, "right"))set_flags( align[num], ASXML_ALIGN_RIGHT);
							else if (!strcmp(tmp->parm, "center"))set_flags( align[num], ASXML_ALIGN_LEFT|ASXML_ALIGN_RIGHT);
						}else if (!strcmp(tmp->tag, "valign"))
						{
							if (!strcmp(tmp->parm, "top"))set_flags( align[num], ASXML_ALIGN_TOP) ;
							else if (!strcmp(tmp->parm, "bottom"))set_flags( align[num], ASXML_ALIGN_BOTTOM);
							else if (!strcmp(tmp->parm, "middle"))set_flags( align[num], ASXML_ALIGN_TOP|ASXML_ALIGN_BOTTOM);
						}
					}
					if (refid) {
						ASImage* refimg = fetch_asimage(imman, refid);
						if (refimg) {
							x = refimg->width;
							y = refimg->height;
						}
						safe_asimage_destroy(refimg );
					}
					x = x_str ? (int)parse_math(x_str, NULL, x) : 0;
					y = y_str ? (int)parse_math(y_str, NULL, y) : 0;
					clip_x = clip_x_str ? (int)parse_math(clip_x_str, NULL, x) : 0;
					clip_y = clip_y_str ? (int)parse_math(clip_y_str, NULL, y) : 0;
					if( clip_width_str )
						clip_width = (int)parse_math(clip_width_str, NULL, clip_width);
					else if( tile )
						clip_width = 0 ;
					if( clip_height_str )
						clip_height = (int)parse_math(clip_height_str, NULL, clip_height);
					else if( tile )
						clip_height = 0 ;
				}
				if (layers[num].im) {
					layers[num].dst_x = x;
					layers[num].dst_y = y;
					layers[num].clip_x = clip_x;
					layers[num].clip_y = clip_y;
					layers[num].clip_width = clip_width ;
					layers[num].clip_height = clip_height ;
					layers[num].tint = tint;
					layers[num].bevel = 0;
					layers[num].merge_scanlines = blend_scanlines_name2func(pop);
					if( clip_width + x > 0 )
					{
						if( width < clip_width + x )
							width = clip_width + x;
					}else
					 	if (width < (int)(layers[num].im->width)) width = layers[num].im->width;
					if( clip_height + y > 0 )
					{
						if( height < clip_height + y )
							height = clip_height + y ;
					}else
						if (height < (int)(layers[num].im->height)) height = layers[num].im->height;
					num++;
				}
				if (sparm) xml_elem_delete(NULL, sparm);
			}

			if (num && merge && layers[0].im ) {
				width = layers[0].im->width;
				height = layers[0].im->height;
			}


	   		for (i = 0 ; i < num ; i++)
			{
				if( get_flags(align[i], ASXML_ALIGN_LEFT|ASXML_ALIGN_RIGHT ) )
				{
					int im_width = ( layers[i].clip_width == 0 )? layers[i].im->width : layers[i].clip_width ;
					int x = 0 ;
					if( get_flags( align[i], ASXML_ALIGN_RIGHT ) )
						x = width - im_width ;
					if( get_flags( align[i], ASXML_ALIGN_LEFT ) )
						x /= 2;
					layers[i].dst_x = x;
				}
				if( get_flags(align[i], ASXML_ALIGN_TOP|ASXML_ALIGN_BOTTOM ) )
				{
					int im_height = ( layers[i].clip_height == 0 )? layers[i].im->height : layers[i].clip_height;
					int y = 0 ;
					if( get_flags( align[i], ASXML_ALIGN_BOTTOM ) )
						y = height - im_height ;
					if( get_flags( align[i], ASXML_ALIGN_TOP ) )
						y /= 2;
					layers[i].dst_y = y;
				}
				if( layers[i].clip_width == 0 )
					layers[i].clip_width = width - layers[i].dst_x;
				if( layers[i].clip_height == 0 )
					layers[i].clip_height = height - layers[i].dst_y;
			}

			show_progress("Compositing [%d] image(s) with op [%s].  Final geometry [%dx%d].", num, pop, width, height);
			if (keep_trans) show_progress("  Keeping transparency.");
			if (verbose > 1) {
				for (i = 0 ; i < num ; i++) {
					show_progress("  Image [%d] geometry [%dx%d+%d+%d]", i, layers[i].clip_width, layers[i].clip_height, layers[i].dst_x, layers[i].dst_y);
					if (layers[i].tint) show_progress(" tint (#%08x)", (unsigned int)layers[i].tint);
				}
			}

			if (num) {
				result = merge_layers( asv, layers, num, width, height, 
									   ASA_ASImage, 0, ASIMAGE_QUALITY_DEFAULT);
				if (keep_trans && result && layers[0].im) {
					copy_asimage_channel(result, IC_ALPHA, layers[0].im, IC_ALPHA);
				}
				while (--num >= 0 )
					safe_asimage_destroy( layers[num].im );
			}
			free(align);
			free(layers);
		}
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);
	}

	if (!strcmp(doc->tag, "printf")) 
	{
		xml_elem_t* parm = xml_parse_parm(doc->parm, NULL);
		const char* format = NULL;
		const char* var = NULL;
		int val = 0 ;
		Bool use_val = False ;
		int arg_count = 0, i;
		for (ptr = parm ; ptr ; ptr = ptr->next) 
		{
			if (!strcmp(ptr->tag, "format")) format = ptr->parm;
			else if (!strcmp(ptr->tag, "var")) { var = ptr->parm; use_val = False; }
			else if (!strcmp(ptr->tag, "val")) { val = (int)parse_math(ptr->parm, NULL, 0); use_val = True; }
		}
   		
		for( i = 0 ; format[i] != '\0' ; ++i )
			if( format[i] == '%' )
			{
				if( format[i+1] != '%' ) 
			 		++arg_count ; 
				else 
					++i ;
			}
		
		if( format != NULL ) 
		{	
			char *interpreted_format = interpret_ctrl_codes( mystrdup(format) );
			if( use_val && arg_count == 1) 
				printf( interpreted_format, val );
			else if( var != NULL && arg_count == 1 ) 
				printf( interpreted_format, asxml_var_get(var) );				
			else if( arg_count == 0 )
				fputs( interpreted_format, stdout );				   
			free( interpreted_format );
		}
		
		if (rparm) *rparm = parm; else xml_elem_delete(NULL, parm);

		if( result == NULL ) 
			return NULL;
	}

	/* No match so far... see if one of our children can do any better.*/
	if (!result) {
		xml_elem_t* tparm = NULL;
		for (ptr = doc->child ; ptr && !result ; ptr = ptr->next) 
		{
			xml_elem_t* sparm = NULL;
			ASImage* imtmp = build_image_from_xml(asv, imman, fontman, ptr, &sparm, flags, verbose, display_win);
			LOCAL_DEBUG_OUT("imtmp = %p", imtmp );
			if (imtmp) 
			{
				if (tparm) xml_elem_delete(NULL, tparm);
				tparm = sparm; 
			}
		}
		if (rparm) *rparm = tparm; 
		else xml_elem_delete(NULL, tparm);
	}

	result = commit_xml_image_built( &state, id, result );
#if 0	
	if (id && result) 
	{
    	char* buf = NEW_ARRAY(char, strlen(id) + 1 + 6 + 1);
		show_progress("Storing image id [%s] with image manager %p .", id, imman);
        sprintf(buf, "%s.width", id);
        asxml_var_insert(buf, result->width);
        sprintf(buf, "%s.height", id);
        asxml_var_insert(buf, result->height);
        free(buf);
		if( result->imageman != NULL )
		{
			ASImage *tmp = clone_asimage(result, SCL_DO_ALL );
			safe_asimage_destroy(result );
			result = tmp ;
		}
		if( result )
		{
			if( !store_asimage( imman, result, id ) )
			{
				show_warning("Failed to store image id [%s].", id);
				safe_asimage_destroy(result );
				result = fetch_asimage( imman, id );
				/*show_warning("Old image with the name fetched as %p.", result);*/
			}else
			{
				/* normally generated image will be destroyed right away, so we need to
			 	* increase ref count, in order to preserve it for future uses : */
				dup_asimage( result );
			}
		}
	}
#endif
	return result;
}


int 
xml_name2id( const char *name, ASHashTable *vocabulary )
{
	ASHashData hdata;
	hdata.i = 0 ;
    get_hash_item(vocabulary, AS_HASHABLE(name), &hdata.vptr); 
	return hdata.i;		
}	 


xml_elem_t* xml_parse_parm(const char* parm, ASHashTable *vocabulary) {
	xml_elem_t* list = NULL;
	const char* eparm;

	if (!parm) return NULL;

	for (eparm = parm ; *eparm ; ) {
		xml_elem_t* p;
		const char* bname;
		const char* ename;
		const char* bval;
		const char* eval;

		/* Spin past any leading whitespace. */
		for (bname = eparm ; isspace((int)*bname) ; bname++);

		/* Check for a parm.  First is the parm name. */
		for (ename = bname ; xml_tagchar((int)*ename) ; ename++);

		/* No name equals no parm equals broken tag. */
		if (!*ename) { eparm = NULL; break; }

		/* No "=" equals broken tag.  We do not support HTML-style parms */
		/* with no value.                                                */
		for (bval = ename ; isspace((int)*bval) ; bval++);
		if (*bval != '=') { eparm = NULL; break; }

		while (isspace((int)*++bval));

		/* If the next character is a quote, spin until we see another one. */
		if (*bval == '"' || *bval == '\'') {
			char quote = *bval;
			bval++;
			for (eval = bval ; *eval && *eval != quote ; eval++);
		} else {
			for (eval = bval ; *eval && !isspace((int)*eval) ; eval++);
		}

		for (eparm = eval ; *eparm && !isspace((int)*eparm) ; eparm++);

		/* Add the parm to our list. */
		p = xml_elem_new();
		if (!list) list = p;
		else { p->next = list; list = p; }
		p->tag = lcstring(mystrndup(bname, ename - bname));
		if( vocabulary )
			p->tag_id = xml_name2id( p->tag, vocabulary );
		p->parm = mystrndup(bval, eval - bval);
	}

	if (!eparm) {
		while (list) {
			xml_elem_t* p = list->next;
			free(list->tag);
			free(list->parm);
			free(list);
			list = p;
		}
	}

	return list;
}

/* The recursive version of xml_print(), so we can indent XML. */
static void xml_print_r(xml_elem_t* root, int depth) {
	xml_elem_t* child;
	if (!strcmp(root->tag, cdata_str)) {
		char* ptr = root->parm;
		while (isspace((int)*ptr)) ptr++;
		fprintf(stderr, "%s", ptr);
	} else {
		fprintf(stderr, "%*s<%s", depth * 2, "", root->tag);
		if (root->parm) {
			xml_elem_t* parm = xml_parse_parm(root->parm, NULL);
			while (parm) {
				xml_elem_t* p = parm->next;
				fprintf(stderr, " %s=\"%s\"", parm->tag, parm->parm);
				free(parm->tag);
				free(parm->parm);
				free(parm);
				parm = p;
			}
		}
		if (root->child) {
			fprintf(stderr, ">\n");
			for (child = root->child ; child ; child = child->next)
				xml_print_r(child, depth + 1);
			fprintf(stderr, "%*s</%s>\n", depth * 2, "", root->tag);
		} else {
			fprintf(stderr, "/>\n");
		}
	}
}

void xml_print(xml_elem_t* root) {
	xml_print_r(root, 0);
}

xml_elem_t* xml_elem_new(void) {
	xml_elem_t* elem = NEW(xml_elem_t);
	elem->next = elem->child = NULL;
	elem->parm = elem->tag = NULL;
	elem->tag_id = XML_UNKNOWN_ID ;
	return elem;
}

xml_elem_t* xml_elem_remove(xml_elem_t** list, xml_elem_t* elem) {
	/* Splice the element out of the list, if it's in one. */
	if (list) {
		if (*list == elem) {
			*list = elem->next;
		} else {
			xml_elem_t* ptr;
			for (ptr = *list ; ptr->next ; ptr = ptr->next) {
				if (ptr->next == elem) {
					ptr->next = elem->next;
					break;
				}
			}
		}
	}
	elem->next = NULL;
	return elem;
}

void xml_elem_delete(xml_elem_t** list, xml_elem_t* elem) {
	if (list) xml_elem_remove(list, elem);
	while (elem) {
		xml_elem_t* ptr = elem;
		elem = elem->next;
		if (ptr->child) xml_elem_delete(NULL, ptr->child);
		if (ptr->tag && ptr->tag != cdata_str && ptr->tag != container_str) free(ptr->tag);
		if (ptr->parm) free(ptr->parm);
		free(ptr);
	}
}

xml_elem_t* xml_parse_doc(const char* str, ASHashTable *vocabulary) {
	xml_elem_t* elem = xml_elem_new();
	elem->tag = container_str;
	elem->tag_id = XML_CONTAINER_ID ;
	xml_parse(str, elem, vocabulary);
	return elem;
}

int xml_parse(const char* str, xml_elem_t* current, ASHashTable *vocabulary) {
	const char* ptr = str;

	/* Find a tag of the form <tag opts>, </tag>, or <tag opts/>. */
	while (*ptr) {
		const char* oab = ptr;

		/* Look for an open oab bracket. */
		for (oab = ptr ; *oab && *oab != '<' ; oab++);

		/* If there are no oab brackets left, we're done. */
		if (*oab != '<') return oab - str;

		/* Does this look like a close tag? */
		if (oab[1] == '/') 
		{
			const char* etag;
			/* Find the end of the tag. */
			for (etag = oab + 2 ; xml_tagchar((int)*etag) ; etag++);

			while (isspace((int)*etag)) ++etag;
			/* If this is an end tag, and the tag matches the tag we're parsing, */
			/* we're done.  If not, continue on blindly. */
			if (*etag == '>') 
			{
				if (!mystrncasecmp(oab + 2, current->tag, etag - (oab + 2))) 
				{
					if (oab - ptr) 
					{
						xml_elem_t* child = xml_elem_new();
						child->tag = cdata_str;
						child->tag_id = XML_CDATA_ID ;
						child->parm = mystrndup(ptr, oab - ptr);
						xml_insert(current, child);
					}
					return (etag + 1) - str;
				}
			}

			/* This tag isn't interesting after all. */
			ptr = oab + 1;
		}

		/* Does this look like a start tag? */
		if (oab[1] != '/') {
			int empty = 0;
			const char* btag = oab + 1;
			const char* etag;
			const char* bparm;
			const char* eparm;

			/* Find the end of the tag. */
			for (etag = btag ; xml_tagchar((int)*etag) ; etag++);

			/* If we reached the end of the document, continue on. */
			if (!*etag) { ptr = oab + 1; continue; }

			/* Find the beginning of the parameters, if they exist. */
			for (bparm = etag ; isspace((int)*bparm) ; bparm++);

			/* From here on, we're looking for a sequence of parms, which have
			 * the form [a-z0-9-]+=("[^"]"|'[^']'|[^ \t\n]), followed by either
			 * a ">" or a "/>". */
			for (eparm = bparm ; *eparm ; ) {
				const char* tmp;

				/* Spin past any leading whitespace. */
				for ( ; isspace((int)*eparm) ; eparm++);

				/* Are we at the end of the tag? */
				if (*eparm == '>' || (*eparm == '/' && eparm[1] == '>')) break;

				/* Check for a parm.  First is the parm name. */
				for (tmp = eparm ; xml_tagchar((int)*tmp) ; tmp++);

				/* No name equals no parm equals broken tag. */
				if (!*tmp) { eparm = NULL; break; }

				/* No "=" equals broken tag.  We do not support HTML-style parms
				   with no value. */
				for ( ; isspace((int)*tmp) ; tmp++);
				if (*tmp != '=') { eparm = NULL; break; }

				while (isspace((int)*++tmp));

				/* If the next character is a quote, spin until we see another one. */
				if (*tmp == '"' || *tmp == '\'') {
					char quote = *tmp;
					for (tmp++ ; *tmp && *tmp != quote ; tmp++);
				}

				/* Now look for a space or the end of the tag. */
				for ( ; *tmp && !isspace((int)*tmp) && *tmp != '>' && !(*tmp == '/' && tmp[1] == '>') ; tmp++);

				/* If we reach the end of the string, there cannot be a '>'. */
				if (!*tmp) { eparm = NULL; break; }

				/* End of the parm.  */
				eparm = tmp;
				
				if (!isspace((int)*tmp)) break; 
				for ( ; isspace((int)*tmp) ; tmp++);
				if( *tmp == '>' || (*tmp == '/' && tmp[1] == '>') )
					break;
			}

			/* If eparm is NULL, the parm string is invalid, and we should
			 * abort processing. */
			if (!eparm) { ptr = oab + 1; continue; }

			/* Save CDATA, if there is any. */
			if (oab - ptr) {
				xml_elem_t* child = xml_elem_new();
				child->tag = cdata_str;
				child->tag_id = XML_CDATA_ID ;
				child->parm = mystrndup(ptr, oab - ptr);
				xml_insert(current, child);
			}

			/* We found a tag!  Advance the pointer. */
			for (ptr = eparm ; isspace((int)*ptr) ; ptr++);
			empty = (*ptr == '/');
			ptr += empty + 1;

			/* Add the tag to our children and parse it. */
			{
				xml_elem_t* child = xml_elem_new();
				child->tag = lcstring(mystrndup(btag, etag - btag));
				if( vocabulary )
					child->tag_id = xml_name2id( child->tag, vocabulary );
				if (eparm - bparm) child->parm = mystrndup(bparm, eparm - bparm);
				xml_insert(current, child);
				if (!empty) ptr += xml_parse(ptr, child, vocabulary);
			}
		}
	}
	return ptr - str;
}

void xml_insert(xml_elem_t* parent, xml_elem_t* child) {
	child->next = NULL;
	if (!parent->child) {
		parent->child = child;
		return;
	}
	for (parent = parent->child ; parent->next ; parent = parent->next);
	parent->next = child;
}


xml_elem_t *
find_tag_by_id( xml_elem_t *chain, int id )
{
	while( chain ) 
	{	
		if( chain->tag_id == id ) 
			return chain;
		chain = chain->next ;
	}
	return NULL ;
}

xml_elem_t *
create_CDATA_tag()	
{ 
	xml_elem_t *cdata = xml_elem_new();
	cdata->tag = mystrdup(XML_CDATA_STR) ;
	cdata->tag_id = XML_CDATA_ID ;
	return cdata;
}

char* lcstring(char* str) {
	char* ptr = str;
	for ( ; *ptr ; ptr++) if (isupper((int)*ptr)) *ptr = tolower((int)*ptr);
	return str;
}

char *interpret_ctrl_codes( char *text )
{
	register char *ptr = text ;
	int len, curr = 0 ;
	if( ptr == NULL )  return NULL ;	

	len = strlen(ptr);
	while( ptr[curr] != '\0' ) 
	{
		if( ptr[curr] == '\\' && ptr[curr+1] != '\0' ) 	
		{
			char subst = '\0' ;
			switch( ptr[curr+1] ) 
			{
				case '\\': subst = '\\' ;  break ;	
				case 'a' : subst = '\a' ;  break ;	 
				case 'b' : subst = '\b' ;  break ;	 
				case 'f' : subst = '\f' ;  break ;	 
				case 'n' : subst = '\n' ;  break ;	 
				case 'r' : subst = '\r' ;  break ;	
				case 't' : subst = '\t' ;  break ;	
				case 'v' : subst = '\v' ;  break ;	 
			}	 
			if( subst ) 
			{
				register int i = curr ; 
				ptr[i] = subst ;
				while( ++i < len ) 
					ptr[i] = ptr[i+1] ; 
				--len ; 
			}
		}	 
		++curr ;
	}	 
	return text;
}	 

void reset_xml_buffer( ASXmlBuffer *xb )
{
	if( xb ) 
	{
		xb->used = 0 ; 
		xb->state = ASXML_Start	 ;
		xb->level = 0 ;
		xb->verbatim = False ;
		xb->quoted = False ;
		xb->tag_type = ASXML_OpeningTag ;
		xb->tags_count = 0 ;
	}		  
}	 


void 
add_xml_buffer_chars( ASXmlBuffer *xb, char *tmp, int len )
{
	if( xb->used + len > xb->allocated ) 
	{	
		xb->allocated = xb->used + (((len>>11)+1)<<11) ;	  
		xb->buffer = realloc( xb->buffer, xb->allocated );
	}
	memcpy( &(xb->buffer[xb->used]), tmp, len );
	xb->used += len ;
}

int 
spool_xml_tag( ASXmlBuffer *xb, char *tmp, int len )
{
	register int i = 0 ; 
	
	if( !xb->verbatim && !xb->quoted && 
		(xb->state != ASXML_Start || xb->level == 0 )) 
	{	/* skip spaces if we are not in string */
		while( i < len && isspace( (int)tmp[i] )) ++i;
		if( i >= len ) 
			return i;
	}
	if( xb->state == ASXML_Start ) 
	{     /* we are looking for the opening '<' */
		if( tmp[i] != '<' ) 
		{
			if( xb->level == 0 ) 	  
				xb->state = ASXML_BadStart ; 
			else
			{
				int start = i ; 
				while( i < len && tmp[i] != '<' ) ++i ;	  
				add_xml_buffer_chars( xb, &tmp[start], i - start );
				return i;
			}
		}else
		{	
			xb->state = ASXML_TagOpen; 	
			xb->tag_type = ASXML_OpeningTag ;
			add_xml_buffer_chars( xb, "<", 1 );
			if( ++i >= len ) 
				return i;
		}
	}
	
	if( xb->state == ASXML_TagOpen ) 
	{     /* we are looking for the beginning of tag name  or closing tag's slash */
		if( tmp[i] == '/' ) 
		{
			xb->state = ASXML_TagName; 
			xb->verbatim = True ; 		   
			xb->tag_type = ASXML_ClosingTag ;
			add_xml_buffer_chars( xb, "/", 1 );
			if( ++i >= len ) 
				return i;
		}else if( isalnum((int)tmp[i]) )	
		{	 
			xb->state = ASXML_TagName; 		   
			xb->verbatim = True ; 		   
		}else
			xb->state = ASXML_BadTagName ;
	}

	if( xb->state == ASXML_TagName ) 
	{     /* we are looking for the tag name */
		int start = i ;
		/* need to store attribute name in form : ' attr_name' */
		while( i < len && isalnum((int)tmp[i]) ) ++i ;
		if( i > start ) 
			add_xml_buffer_chars( xb, &tmp[start], i - start );
		if( i < len ) 
		{	
			if( isspace( (int)tmp[i] ) || tmp[i] == '>' ) 
			{
				xb->state = ASXML_TagAttrOrClose;
				xb->verbatim = False ; 
			}else
				xb->state = ASXML_BadTagName ;
		}			 
		return i;
	}

	if( xb->state == ASXML_TagAttrOrClose ) 
	{   /* we are looking for the atteribute or closing '/>' or '>' */
		Bool has_slash = (xb->tag_type != ASXML_OpeningTag);

		if( !has_slash && tmp[i] == '/' )
		{	
			xb->tag_type = ASXML_SimpleTag ;
			add_xml_buffer_chars( xb, "/", 1 );		 			  
			++i ;
			has_slash = True ;
		}
		if( i < len ) 
		{	
			if( has_slash && tmp[i] != '>') 
				xb->state = ASXML_UnexpectedSlash ;	  
			else if( tmp[i] == '>' ) 
			{
				++(xb->tags_count);
				xb->state = ASXML_Start; 	
	 			add_xml_buffer_chars( xb, ">", 1 );		 			  
				++i ;
				if( xb->tag_type == ASXML_OpeningTag )
					++(xb->level);
				else if( xb->tag_type == ASXML_ClosingTag )					
				{
					if( xb->level <= 0 )
					{
				 		xb->state = ASXML_UnmatchedClose;
						return i;		   
					}else
						--(xb->level);			
				}		 			   
			}else if( !isalnum( (int)tmp[i] ) )	  
				xb->state = ASXML_BadAttrName ;
			else
			{	
				xb->state = ASXML_AttrName;		 
				xb->verbatim = True ;
				add_xml_buffer_chars( xb, " ", 1);
			}
		}
		return i;
	}

	if( xb->state == ASXML_AttrName ) 
	{	
		int start = i ;
		/* need to store attribute name in form : ' attr_name' */
		while( i < len && isalnum((int)tmp[i]) ) ++i ;
		if( i > start ) 
			add_xml_buffer_chars( xb, &tmp[start], i - start );
		if( i < len ) 
		{	
			if( isspace( (int)tmp[i] ) || tmp[i] == '=' ) 
			{
				xb->state = ASXML_AttrEq;
				xb->verbatim = False ; 
				/* should fall down to case below */
			}else
				xb->state = ASXML_BadAttrName ;
		}
	 	return i;				 
	}	

	if( xb->state == ASXML_AttrEq )                   /* looking for '=' */
	{
		if( tmp[i] == '=' ) 
		{
			xb->state = ASXML_AttrValueStart;				
			add_xml_buffer_chars( xb, "=", 1 );		 			  
			++i ;
		}else	 
			xb->state = ASXML_MissingAttrEq ;
		return i;
	}	
	
	if( xb->state == ASXML_AttrValueStart )/*looking for attribute value:*/
	{
		xb->state = ASXML_AttrValue ;
		if( tmp[i] == '"' )
		{
			xb->quoted = True ; 
			add_xml_buffer_chars( xb, "\"", 1 );
			++i ;
		}else	 
			xb->verbatim = True ; 
		return i;
	}	  
	
	if( xb->state == ASXML_AttrValue )  /* looking for attribute value : */
	{
		if( !xb->quoted && isspace((int)tmp[i]) ) 
		{
			add_xml_buffer_chars( xb, " ", 1 );
			++i ;
			xb->verbatim = False ; 
			xb->state = ASXML_TagAttrOrClose ;
		}else if( xb->quoted && tmp[i] == '"' ) 
		{
			add_xml_buffer_chars( xb, "\"", 1 );
			++i ;
			xb->quoted = False ; 
			xb->state = ASXML_TagAttrOrClose ;
		}else if( tmp[i] == '/' && !xb->quoted)
		{
			xb->state = ASXML_AttrSlash ;				
			add_xml_buffer_chars( xb, "/", 1 );		 			  
			++i ;
		}else if( tmp[i] == '>' )
		{
			xb->quoted = False ; 
			xb->verbatim = False ; 
			xb->state = ASXML_TagAttrOrClose ;				
		}else			
		{
			add_xml_buffer_chars( xb, &tmp[i], 1 );
			++i ;
		}
		return i;
	}	  
	if( xb->state == ASXML_AttrSlash )  /* looking for attribute value : */
	{
		if( tmp[i] == '>' )
		{
			xb->tag_type = ASXML_SimpleTag ;
			add_xml_buffer_chars( xb, ">", 1 );		 			  
			++i ;
			++(xb->tags_count);
			xb->state = ASXML_Start; 	
			xb->quoted = False ; 
			xb->verbatim = False ; 
		}else
		{
			xb->state = ASXML_AttrValue ;
		}		 
		return i;
	}

	return (i==0)?1:i;
}	   

char 
translate_special_sequence( const char *ptr, int len,  int *seq_len )
{
	int c = '\0' ;
	int c_len = 0 ;
	if( ptr[0] == '&') 
	{ 
		if( 4 <= len ) 
		{	
			c_len = 4 ;
			if( strncmp(ptr,"&lt;", c_len ) == 0 ) c = '<' ;
			else if( strncmp(ptr,"&gt;", c_len ) == 0 ) c = '>' ;
		}
		if( c == '\0' && 5 <= len ) 
		{	
			c_len = 5 ;
			if( strncmp(ptr,"&amp;", c_len ) == 0 ) c = '&' ;
		}
				
		if( c == '\0' && 6 <= len ) 
		{	
			c_len = 6 ;
			if(      strncmp(ptr,"&quot;", c_len ) == 0 ) c = '"' ;
			else if( strncmp(ptr,"&circ;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&nbsp;", c_len ) == 0 ) c = ' ' ;
			else if( strncmp(ptr,"&ensp;", c_len ) == 0 ) c = ' ' ;
			else if( strncmp(ptr,"&emsp;", c_len ) == 0 ) c = ' ' ;
			else if( strncmp(ptr,"&Yuml;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&euro;", c_len ) == 0 ) c = '�' ;					 
		}

		if( c == '\0' && 7 <= len ) 
		{	
			c_len = 7 ;
			if( strncmp(ptr,"&OElig;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&oelig;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&tilde;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&ndash;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&mdash;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&lsquo;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&rsquo;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&sbquo;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&ldquo;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&rdquo;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&bdquo;", c_len ) == 0 ) c = '�' ;
		}				
		if( c == '\0' && 8 <= len ) 
		{	
			c_len = 8 ;
			if( strncmp(ptr,"&Scaron;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&scaron;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&thinsp;", c_len ) == 0 ) c = ' ' ;
			else if( strncmp(ptr,"&dagger;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&Dagger;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&permil;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&lsaquo;", c_len ) == 0 ) c = '�' ;
			else if( strncmp(ptr,"&rsaquo;", c_len ) == 0 ) c = '�' ;
		}
	}		
						
	if( seq_len )    
		*seq_len = (c == '\0')?0:c_len ;
	return c;   				 
}

void	   
append_cdata( xml_elem_t *cdata_tag, const char *line, int len )
{
	int i, k; 
	int tabs_count = 0 ;
	int old_length = 0;
	char *ptr ;
	
	for( i = 0 ; i < len ; ++i ) 
		if( line[i] == '\t' )
			++tabs_count ;
	if( cdata_tag->parm ) 
		old_length = strlen(cdata_tag->parm);

	cdata_tag->parm = realloc( cdata_tag->parm, old_length + 1 + len + tabs_count*3+1);
	ptr = &(cdata_tag->parm[old_length]) ;
	if( old_length > 0 )
		if( cdata_tag->parm[old_length-1] != '\n') 
		{	
			ptr[0] = '\n' ;
			++ptr ;
		}
	k = 0 ;
	for( i = 0 ; i < len ; ++i ) 
	{	
		if( line[i] == '\t' )
		{
			int tab_stop = (((k+3)/4)*4) ; 
			if( tab_stop == k ) 
				tab_stop += 4 ;
/*			fprintf( stderr, "k = %d, tab_stop = %d, len = %d\n", k, tab_stop, len ); */
			while( k < tab_stop )
				ptr[k++] = ' ' ;
		}else if( line[i] == '\n' )
		{
			ptr[k] = '\n' ;
			ptr += k+1 ;
			k = 0 ;	  
		}else
			ptr[k++] = line[i] ;
	}		
	ptr[k] = '\0';
}	 


void 
append_CDATA_line( xml_elem_t *tag, const char *line, int len )
{
	xml_elem_t *cdata_tag = find_tag_by_id(tag->child, XML_CDATA_ID );
	LOCAL_DEBUG_CALLER_OUT("tag->tag = \"%s\", line_len = %d", tag->tag, len );

	if( cdata_tag == NULL ) 
	{
		cdata_tag = create_CDATA_tag() ;
		xml_insert(tag, cdata_tag);
	}	 
	append_cdata( cdata_tag, line, len );
}


