/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mimetypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 08:00:23 by badam             #+#    #+#             */
/*   Updated: 2022/04/14 01:34:06 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MIMETYPES_CPP
# define __MIMETYPES_CPP

# include <map>
# include "IMiddleware.hpp"
# include "Response.hpp"
# include "Request.hpp"

class Mimetypes: public IMiddleware
{
	typedef	IMiddleware							_parent;
	typedef	std::map<std::string, std::string>	mimetypes_t;

	public:
		typedef struct	options_s
		{
			std::string	fallback;
			std::string	directory_default;
			mimetypes_t	mimetypes;
		}				options_t;
		
		options_t		options;


		Mimetypes()
		{
			options.fallback = "";
			options.directory_default = "text/html";
		}

		Mimetypes(options_t opts)
		{
			options = opts;
		}

		virtual ~Mimetypes()
		{}

		void	useDefaults()
		{
			options.mimetypes.clear();
			
			add("html", "text/html");
			add("htm", "text/html");
			add("shtml", "text/html");
			add("css", "text/css");
			add("xml", "text/xml");
			add("gif", "image/gif");
			add("jpeg", "image/jpeg");
			add("jpg", "image/jpeg");
			add("js", "application/javascript");
			add("atom", "application/atom+xml");
			add("rss", "application/rss+xml");
			add("mml", "text/mathml");
			add("txt", "text/plain");
			add("jad", "text/vnd.sun.j2me.app-descriptor");
			add("wml", "text/vnd.wap.wml");
			add("htc", "text/x-component");
			add("avif", "image/avif");
			add("png", "image/png");
			add("svg", "image/svg+xml");
			add("svgz", "image/svg+xml");
			add("tif", "image/tiff");
			add("tiff", "image/tiff");
			add("wbmp", "image/vnd.wap.wbmp");
			add("webp", "image/webp");
			add("ico", "image/x-icon");
			add("jng", "image/x-jng");
			add("bmp", "image/x-ms-bmp");
			add("woff", "font/woff");
			add("woff2", "font/woff2");
			add("jar", "application/java-archive");
			add("war", "application/java-archive");
			add("ear", "application/java-archive");
			add("json", "application/json");
			add("hqx", "application/mac-binhex40");
			add("doc", "application/msword");
			add("pdf", "application/pdf");
			add("ps", "application/postscript");
			add("eps", "application/postscript");
			add("ai", "application/postscript");
			add("rtf", "application/rtf");
			add("m3u8", "application/vnd.apple.mpegurl");
			add("kml", "application/vnd.google-earth.kml+xml");
			add("kmz", "application/vnd.google-earth.kmz");
			add("xls", "application/vnd.ms-excel");
			add("eot", "application/vnd.ms-fontobject");
			add("ppt", "application/vnd.ms-powerpoint");
			add("odg", "application/vnd.oasis.opendocument.graphics");
			add("odp", "application/vnd.oasis.opendocument.presentation");
			add("ods", "application/vnd.oasis.opendocument.spreadsheet");
			add("odt", "application/vnd.oasis.opendocument.text");
			add("pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation");
			add("xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
			add("docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
			add("wmlc", "application/vnd.wap.wmlc");
			add("wasm", "application/wasm");
			add("7z", "application/x-7z-compressed");
			add("cco", "application/x-cocoa");
			add("jardiff", "application/x-java-archive-diff");
			add("jnlp", "application/x-java-jnlp-file");
			add("run", "application/x-makeself");
			add("pl", "application/x-perl");
			add("pm", "application/x-perl");
			add("prc", "application/x-pilot");
			add("pdb", "application/x-pilot");
			add("rar", "application/x-rar-compressed");
			add("rpm", "application/x-redhat-package-manager");
			add("sea", "application/x-sea");
			add("swf", "application/x-shockwave-flash");
			add("sit", "application/x-stuffit");
			add("tcl", "application/x-tcl");
			add("tk", "application/x-tcl");
			add("der", "application/x-x509-ca-cert");
			add("pem", "application/x-x509-ca-cert");
			add("crt", "application/x-x509-ca-cert");
			add("xpi", "application/x-xpinstall");
			add("xhtml", "application/xhtml+xml");
			add("xspf", "application/xspf+xml");
			add("zip", "application/zip");
			add("bin", "application/octet-stream");
			add("exe", "application/octet-stream");
			add("dll", "application/octet-stream");
			add("deb", "application/octet-stream");
			add("dmg", "application/octet-stream");
			add("iso", "application/octet-stream");
			add("img", "application/octet-stream");
			add("msi", "application/octet-stream");
			add("msp", "application/octet-stream");
			add("msm", "application/octet-stream");
			add("mid", "audio/midi");
			add("midi", "audio/midi");
			add("kar", "audio/midi");
			add("mp3", "audio/mpeg");
			add("ogg", "audio/ogg");
			add("m4a", "audio/x-m4a");
			add("ra", "audio/x-realaudio");
			add("3gpp", "video/3gpp");
			add("3gp", "video/3gpp");
			add("ts", "video/mp2t");
			add("mp4", "video/mp4");
			add("mpeg", "video/mpeg");
			add("mpg", "video/mpeg");
			add("mov", "video/quicktime");
			add("webm", "video/webm");
			add("flv", "video/x-flv");
			add("m4v", "video/x-m4v");
			add("mng", "video/x-mng");
			add("asx", "video/x-ms-asf");
			add("asf", "video/x-ms-asf");
			add("wmv", "video/x-ms-wmv");
			add("avi", "video/x-msvideo");
		}

		void	add(std::string ext, std::string mimetype)
		{
			options.mimetypes.insert(std::pair<std::string, std::string>(ext, mimetype));
		}

		std::string	getMimetype(std::string &path)
		{
			mimetypes_t::const_iterator	it;

			if (isDirectory(path))
				return (options.directory_default);
			
			it = options.mimetypes.find(getExtension(path));

			if (it == options.mimetypes.end())
				return (options.fallback);
			else
				return it->second;
		}

	public:
		bool	operator()(Request &req, Response &res)
		{
			std::string	&path	= res.used_file.length() ? res.used_file : req.trusted_pathname;

			if (res.code != C_NO_CONTENT && res.code != C_CREATED)
			{
				std::string	mimetype	= getMimetype(path);

				if (mimetype.length())
					res.headers.add("Content-Type: " + mimetype);
			}
			
			return (true);
		}

};

#endif
