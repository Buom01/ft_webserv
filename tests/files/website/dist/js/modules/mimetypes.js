"use strict";var db=require("./mimedb.json"),extname=require("path").extname,EXTRACT_TYPE_REGEXP=/^\s*([^;\s]*)(?:;|\s|$)/,TEXT_TYPE_REGEXP=/^text\//i;function charset(e){if(!e||"string"!=typeof e)return!1;var t=EXTRACT_TYPE_REGEXP.exec(e),e=t&&db[t[1].toLowerCase()];return e&&e.charset?e.charset:!(!t||!TEXT_TYPE_REGEXP.test(t[1]))&&"UTF-8"}function contentType(e){if(!e||"string"!=typeof e)return!1;var t=-1===e.indexOf("/")?exports.lookup(e):e;return!!t&&(-1!==t.indexOf("charset")||(e=exports.charset(t))&&(t+="; charset="+e.toLowerCase()),t)}function extension(e){if(!e||"string"!=typeof e)return!1;e=EXTRACT_TYPE_REGEXP.exec(e),e=e&&exports.extensions[e[1].toLowerCase()];return!(!e||!e.length)&&e[0]}function lookup(e){if(!e||"string"!=typeof e)return!1;e=extname("x."+e).toLowerCase().substr(1);return e&&exports.types[e]||!1}function populateMaps(i,p){var c=["nginx","apache",void 0,"iana"];Object.keys(db).forEach(function(e){var t=db[e],r=t.extensions;if(r&&r.length){i[e]=r;for(var n=0;n<r.length;n++){var o=r[n];if(p[o]){var s=c.indexOf(db[p[o]].source),a=c.indexOf(t.source);if("application/octet-stream"!==p[o]&&(a<s||s===a&&"application/"===p[o].substr(0,12)))continue}p[o]=e}}})}exports.charset=charset,exports.charsets={lookup:charset},exports.contentType=contentType,exports.extension=extension,exports.extensions=Object.create(null),exports.lookup=lookup,exports.types=Object.create(null),populateMaps(exports.extensions,exports.types);