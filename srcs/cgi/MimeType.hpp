#ifndef __MIME_TYPE
# define __MIME_TYPE
# define MIMESIZE 17
# include <string>

struct s_mimeType
{
	std::string extension;
	std::string	type;
} __mimetype[MIMESIZE] = 
{
	{".binary", "binary/octet-stream"},
	{".css", "text/css"},
	{".doc", "application/msword"},
	{".eot", "application/vnd.ms-fontobject"},
	{".html", "text/html"},
	{".ico", "image/x-icon"},
	{".jpg", "image/jpeg"},
	{".js", "text/javascript"},
	{".json", "application/json"},
	{".mp3", "audio/mpeg"},
	{".pdf", "application/pdf"},
	{".php", "text/php"},
	{".png", "image/png"},
	{".svg", "image/svg+xml"},
	{".ttf", "application/font-sfnt"},
	{".wav", "audio/wav"},
	{".webp", "image/webp"},
};

/**
 * Get correct mimetype for file extension
 * @return s_mimeType, empty if not found
 */
s_mimeType getMimeType(std::string extension)
{
	for (size_t x = 0; x < MIMESIZE; x++)
	{
		if (__mimetype[x].extension == extension)
			return __mimetype[x];
	}
	return s_mimeType();
}
#endif
