/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usefull.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:23 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/19 13:26:34 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USEFULL_HPP
# define USEFULL_HPP
# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define YELLOW "\x1B[33m"
# define ORANGE "\x1B[34m"
# define PURPLE "\x1B[35m"
# define BLUE "\x1B[36m"
# define BOLD "\x1B[1m"
# define GRAY "\x1B[90m"
# define END "\x1B[0m"
# define ERR -1
# define OK 0
# define LOG 0
# define DIRECTORY 0
# define REGFILE 1

// librairies
# include <iostream>
# include <sstream>
# include <fstream>

# include <vector>
# include <map>
# include <string>
# include <algorithm>

# include <limits>
# include <utility>

# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <time.h>
# include <fcntl.h>
# include <errno.h>

# include <cstdlib>
# include <cstdio>

# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/epoll.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>

typedef std::vector<std::string>					vecStr;
typedef std::map<int, std::string>					mapErr;
typedef	std::vector<int>							vecInt;
typedef	std::map<int, int>							mapSock;
typedef std::vector< std::pair<std::string, int> >	vecFiles;
typedef std::pair<std::string, std::string> 		pairStr;
typedef struct dirent fileInfo;
typedef DIR	directory;

bool	splitBlocks(vecStr &splitted, std::string str, std::string pattern, std::string &otherInfo);
void	splitPattern(vecStr &splitted, std::string str, std::string pattern);

void	printFormatError(void);
void	printFormatError(std::string);
void	printFileError(std::string file);

bool	checkHostFormat(std::string str);
bool	checkWordFormat(std::string str);

size_t	myAtoi(std::string str);
size_t	myAtoi(std::string str, bool &success);

int				getMethodNb(std::string method);
std::string		getMethod(int methodNm);
std::ostream	&showMethod(std::ostream &o, vecInt methods);

std::string		fileName(int argc, char **argv);
std::string 	copystr(std::string str, int start);
vecStr			splitThis(std::string str);
std::string		removeSlash(const std::string &str);
std::string		toString(int nb);
std::string		toString(size_t nb);
std::string		toString(long nb);

bool			isDirectory(std::string path);
bool			isRegFile(std::string path);

template<typename T> std::ostream	&operator<<(std::ostream &o, std::vector<T> vect)
{
	for (size_t i = 0; i + 1 < vect.size(); i++)
		o << vect[i] << " ";
	if (vect.size() > 0)
		o << vect[vect.size() - 1];
	return (o);
}

std::ostream	&operator<<(std::ostream &o, mapErr map);
std::string	mimeContentType(std::string accepted, std::string extension);

static const int			nbMethods = 3;
static const std::string	methods[nbMethods] = {"GET", "POST", "DELETE"};

static const int			nbMime = 347;
static const std::string	mimeExt[nbMime] = {"3gp", "jpm", "mp3", "rtf", "wav", "xml",
	"3g2", "3gp", "3gpp", "ac", "adp", "ai", "apng", "appcache", "asc", "atom", "atomcat",
	"atomsvc", "au", "aw", "bdoc", "bin", "bmp", "bpk", "buffer", "ccxml", "cdmia", "cdmic",
	"cdmid", "cdmio", "cdmiq", "cer", "cgm", "class", "coffee", "conf", "cpt", "crl", "css",
	"csv", "cu", "davmount", "dbk", "deb", "def", "deploy", "disposition-notification",
	"dist", "distz", "dll", "dmg", "dms", "doc", "dot", "drle", "dssc", "dtd", "dump", "ear",
	"ecma", "elc", "emf", "eml", "emma", "eps", "epub", "es", "exe", "exi", "exr", "ez",
	"fits", "g3", "gbr", "geojson", "gif", "glb", "gltf", "gml", "gpx", "gram", "grxml",
	"gxf", "gz", "h261", "h263", "h264", "heic", "heics", "heif", "heifs", "hjson", "hlp",
	"hqx", "htm", "html", "ics", "ief", "ifb", "iges", "igs", "img", "in", "ini", "ink",
	"inkml", "ipfix", "iso", "jade", "jar", "jls", "jp2", "jpe", "jpeg", "jpf", "jpg", "jpg2",
	"jpgm", "jpgv", "jpm", "jpx", "js", "json", "json5", "jsonld", "jsonml", "jsx", "kar",
	"ktx", "less", "list", "litcoffee", "log", "lostxml", "lrf", "m1v", "m21", "m2a", "m2v",
	"m3a", "m4a", "m4p", "ma", "mads", "man", "manifest", "map", "mar", "markdown", "mathml",
	"mb", "mbox", "md", "me", "mesh", "meta4", "metalink", "mets", "mft", "mid", "midi",
	"mime", "mj2", "mjp2", "mjs", "mml", "mods", "mov", "mp2", "mp21", "mp2a", "mp3", "mp4",
	"mp4a", "mp4s", "mp4v", "mpd", "mpe", "mpeg", "mpg", "mpg4", "mpga", "mrc", "mrcx", "ms",
	"mscml", "msh", "msi", "msm", "msp", "mxf", "mxml", "n3", "nb", "oda", "oga", "ogg",
	"ogv", "ogx", "omdoc", "onepkg", "onetmp", "onetoc", "onetoc2", "opf", "otf", "owl",
	"oxps", "p10", "p7c", "p7m", "p7s", "p8", "pdf", "pfr", "pgp", "pkg", "pki", "pkipath",
	"pls", "png", "prf", "ps", "pskcxml", "qt", "raml", "rdf", "rif", "rl", "rld", "rmi",
	"rnc", "rng", "roa", "roff", "rq", "rs", "rsd", "rss", "rtf", "rtx", "s3m", "sbml", "scq",
	"scs", "sdp", "ser", "setpay", "setreg", "sgi", "sgm", "sgml", "shex", "shf", "shtml",
	"sig", "sil", "silo", "slim", "slm", "smi", "smil", "snd", "so", "spp", "spq", "spx",
	"sru", "srx", "ssdl", "ssml", "stk", "styl", "stylus", "svg", "svgz", "t", "t38", "tei",
	"teicorpus", "text", "tfi", "tfx", "tif", "tiff", "tr", "ts", "tsd", "tsv", "ttc", "ttf",
	"ttl", "txt", "u8dsn", "u8hdr", "u8mdn", "u8msg", "uri", "uris", "urls", "vcard", "vrml",
	"vtt", "vxml", "war", "wasm", "wav", "weba", "webm", "webmanifest", "webp", "wgt", "wmf",
	"woff", "woff2", "wrl", "wsdl", "wspolicy", "x3d", "x3db", "x3dbz", "x3dv", "x3dvz",
	"x3dz", "xaml", "xdf", "xdssc", "xenc", "xer", "xht", "xhtml", "xhvml", "xm", "xml", "xop",
	"xpl", "xsd", "xsl", "xslt", "xspf", "xvm", "xvml", "yaml", "yang", "yin", "yml", "zip"
};

static const std::string	mimeType[nbMime] = { "audio/3gpp", "video/jpm", "audio/mp3",
	"text/rtf", "audio/wave", "text/xml", "video/3gpp2", "video/3gpp", "video/3gpp",
	"application/pkix-attr-cert", "audio/adpcm", "application/postscript", "image/apng",
	"text/cache-manifest", "application/pgp-signature", "application/atom+xml",
	"application/atomcat+xml", "application/atomsvc+xml", "audio/basic",
	"application/applixware", "application/bdoc", "application/octet-stream", "image/bmp",
	"application/octet-stream", "application/octet-stream", "application/ccxml+xml",
	"application/cdmi-capability", "application/cdmi-container", "application/cdmi-domain",
	"application/cdmi-object", "application/cdmi-queue", "application/pkix-cert", "image/cgm",
	"application/java-vm", "text/coffeescript", "text/plain", "application/mac-compactpro",
	"application/pkix-crl", "text/css", "text/csv", "application/cu-seeme",
	"application/davmount+xml", "application/docbook+xml", "application/octet-stream",
	"text/plain", "application/octet-stream", "message/disposition-notification",
	"application/octet-stream", "application/octet-stream", "application/octet-stream",
	"application/octet-stream", "application/octet-stream", "application/msword",
	"application/msword", "image/dicom-rle", "application/dssc+der", "application/xml-dtd",
	"application/octet-stream", "application/java-archive", "application/ecmascript",
	"application/octet-stream", "image/emf", "message/rfc822", "application/emma+xml",
	"application/postscript", "application/epub+zip", "application/ecmascript",
	"application/octet-stream", "application/exi", "image/aces", "application/andrew-inset",
	"image/fits", "image/g3fax", "application/rpki-ghostbusters", "application/geo+json",
	"image/gif", "model/gltf-binary", "model/gltf+json", "application/gml+xml",
	"application/gpx+xml", "application/srgs", "application/srgs+xml", "application/gxf",
	"application/gzip", "video/h261", "video/h263", "video/h264", "image/heic",
	"image/heic-sequence", "image/heif", "image/heif-sequence", "application/hjson",
	"application/winhlp", "application/mac-binhex40", "text/html", "text/html", "text/calendar",
	"image/ief", "text/calendar", "model/iges", "model/iges", "application/octet-stream",
	"text/plain", "text/plain", "application/inkml+xml", "application/inkml+xml",
	"application/ipfix", "application/octet-stream", "text/jade", "application/java-archive",
	"image/jls", "image/jp2", "image/jpeg", "image/jpeg", "image/jpx", "image/jpeg",
	"image/jp2", "video/jpm", "video/jpeg", "image/jpm", "image/jpx", "application/javascript",
	"application/json", "application/json5", "application/ld+json", "application/jsonml+json",
	"text/jsx", "audio/midi", "image/ktx", "text/less", "text/plain", "text/coffeescript",
	"text/plain", "application/lost+xml", "application/octet-stream", "video/mpeg",
	"application/mp21", "audio/mpeg", "video/mpeg", "audio/mpeg", "audio/mp4", "application/mp4",
	"application/mathematica", "application/mads+xml", "text/troff", "text/cache-manifest",
	"application/json", "application/octet-stream", "text/markdown", "application/mathml+xml",
	"application/mathematica", "application/mbox", "text/markdown", "text/troff", "model/mesh",
	"application/metalink4+xml", "application/metalink+xml", "application/mets+xml",
	"application/rpki-manifest", "audio/midi", "audio/midi", "message/rfc822", "video/mj2",
	"video/mj2", "application/javascript", "text/mathml", "application/mods+xml",
	"video/quicktime", "audio/mpeg", "application/mp21", "audio/mpeg", "audio/mpeg", "video/mp4",
	"audio/mp4", "application/mp4", "video/mp4", "application/dash+xml", "video/mpeg",
	"video/mpeg", "video/mpeg", "video/mp4", "audio/mpeg", "application/marc",
	"application/marcxml+xml", "text/troff", "application/mediaservercontrol+xml", "model/mesh",
	"application/octet-stream", "application/octet-stream", "application/octet-stream",
	"application/mxf", "application/xv+xml", "text/n3", "application/mathematica",
	"application/oda", "audio/ogg", "audio/ogg", "video/ogg", "application/ogg",
	"application/omdoc+xml", "application/onenote", "application/onenote", "application/onenote",
	"application/onenote", "application/oebps-package+xml", "font/otf", "application/rdf+xml",
	"application/oxps", "application/pkcs10", "application/pkcs7-mime", "application/pkcs7-mime",
	"application/pkcs7-signature", "application/pkcs8", "application/pdf", "application/font-tdpfr",
	"application/pgp-encrypted", "application/octet-stream", "application/pkixcmp",
	"application/pkix-pkipath", "application/pls+xml", "image/png", "application/pics-rules",
	"application/postscript", "application/pskc+xml", "video/quicktime", "application/raml+yaml",
	"application/rdf+xml", "application/reginfo+xml", "application/resource-lists+xml",
	"application/resource-lists-diff+xml", "audio/midi", "application/relax-ng-compact-syntax",
	"application/xml", "application/rpki-roa", "text/troff", "application/sparql-query",
	"application/rls-services+xml", "application/rsd+xml", "application/rss+xml", "application/rtf",
	"text/richtext", "audio/s3m", "application/sbml+xml", "application/scvp-cv-request",
	"application/scvp-cv-response", "application/sdp", "application/java-serialized-object",
	"application/set-payment-initiation", "application/set-registration-initiation", "image/sgi",
	"text/sgml", "text/sgml", "text/shex", "application/shf+xml", "text/html",
	"application/pgp-signature", "audio/silk", "model/mesh", "text/slim", "text/slim",
	"application/smil+xml", "application/smil+xml", "audio/basic", "application/octet-stream",
	"application/scvp-vp-response", "application/scvp-vp-request", "audio/ogg",
	"application/sru+xml", "application/sparql-results+xml", "application/ssdl+xml",
	"application/ssml+xml", "application/hyperstudio", "text/stylus", "text/stylus",
	"image/svg+xml", "image/svg+xml", "text/troff", "image/t38", "application/tei+xml",
	"application/tei+xml", "text/plain", "application/thraud+xml", "image/tiff-fx", "image/tiff",
	"image/tiff", "text/troff", "video/mp2t", "application/timestamped-data",
	"text/tab-separated-values", "font/collection", "font/ttf", "text/turtle", "text/plain",
	"message/global-delivery-status", "message/global-headers",
	"message/global-disposition-notification", "message/global", "text/uri-list", "text/uri-list",
	"text/uri-list", "text/vcard", "model/vrml", "text/vtt", "application/voicexml+xml",
	"application/java-archive", "application/wasm", "audio/wav", "audio/webm", "video/webm",
	"application/manifest+json", "image/webp", "application/widget", "image/wmf", "font/woff",
	"font/woff2", "model/vrml", "application/wsdl+xml", "application/wspolicy+xml", "model/x3d+xml",
	"model/x3d+binary", "model/x3d+binary", "model/x3d+vrml", "model/x3d+vrml", "model/x3d+xml",
	"application/xaml+xml", "application/xcap-diff+xml", "application/dssc+xml",
	"application/xenc+xml", "application/patch-ops-error+xml", "application/xhtml+xml",
	"application/xhtml+xml", "application/xv+xml", "audio/xm", "application/xml",
	"application/xop+xml", "application/xproc+xml", "application/xml", "application/xml",
	"application/xslt+xml", "application/xspf+xml", "application/xv+xml", "application/xv+xml",
	"text/yaml", "application/yang", "application/yin+xml", "text/yaml", "application/zip"
};

#endif
