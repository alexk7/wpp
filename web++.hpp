#include <dirent.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>

#include <climits>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#define SERVER_NAME "Web++"
#define SERVER_VERSION "1.0.1"
#define BUFSIZE 8096

namespace WPP {
    inline void split(std::string str, std::string separator, int max, std::vector<std::string>* results){
        int i = 0;
        size_t found = str.find_first_of(separator);

        while (found != std::string::npos){
            if (found > 0)
                results->push_back(str.substr(0, found));
            str = str.substr(found+1);
            found = str.find_first_of(separator);

            if (max > -1 && ++i == max)
				break;
        }

        if (str.length() > 0){
            results->push_back(str);
        }
    }
    inline std::string trim(std::string s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());

        return s;
    }
    inline std::map<std::string, std::string> create_mime() {
		return {
			{ "atom", "application/atom+xml" },
			{ "hqx", "application/mac-binhex40" },
			{ "cpt", "application/mac-compactpro" },
			{ "mathml", "application/mathml+xml" },
			{ "doc", "application/msword" },
			{ "bin", "application/octet-stream" },
			{ "dms", "application/octet-stream" },
			{ "lha", "application/octet-stream" },
			{ "lzh", "application/octet-stream" },
			{ "exe", "application/octet-stream" },
			{ "class", "application/octet-stream" },
			{ "so", "application/octet-stream" },
			{ "dll", "application/octet-stream" },
			{ "dmg", "application/octet-stream" },
			{ "oda", "application/oda" },
			{ "ogg", "application/ogg" },
			{ "pdf", "application/pdf" },
			{ "ai", "application/postscript" },
			{ "eps", "application/postscript" },
			{ "ps", "application/postscript" },
			{ "rdf", "application/rdf+xml" },
			{ "smi", "application/smil" },
			{ "smil", "application/smil" },
			{ "gram", "application/srgs" },
			{ "grxml", "application/srgs+xml" },
			{ "air", "application/vnd.adobe.apollo-application-installer-package+zip" },
			{ "mif", "application/vnd.mif" },
			{ "xul", "application/vnd.mozilla.xul+xml" },
			{ "xls", "application/vnd.ms-excel" },
			{ "ppt", "application/vnd.ms-powerpoint" },
			{ "rm", "application/vnd.rn-realmedia" },
			{ "wbxml", "application/vnd.wap.wbxml" },
			{ "wmlc", "application/vnd.wap.wmlc" },
			{ "wmlsc", "application/vnd.wap.wmlscriptc" },
			{ "vxml", "application/voicexml+xml" },
			{ "bcpio", "application/x-bcpio" },
			{ "vcd", "application/x-cdlink" },
			{ "pgn", "application/x-chess-pgn" },
			{ "cpio", "application/x-cpio" },
			{ "csh", "application/x-csh" },
			{ "dcr", "application/x-director" },
			{ "dir", "application/x-director" },
			{ "dxr", "application/x-director" },
			{ "dvi", "application/x-dvi" },
			{ "spl", "application/x-futuresplash" },
			{ "gtar", "application/x-gtar" },
			{ "hdf", "application/x-hdf" },
			{ "js", "application/x-javascript" },
			{ "latex", "application/x-latex" },
			{ "sh", "application/x-sh" },
			{ "shar", "application/x-shar" },
			{ "swf", "application/x-shockwave-flash" },
			{ "sit", "application/x-stuffit" },
			{ "sv4cpio", "application/x-sv4cpio" },
			{ "sv4crc", "application/x-sv4crc" },
			{ "tar", "application/x-tar" },
			{ "tcl", "application/x-tcl" },
			{ "tex", "application/x-tex" },
			{ "man", "application/x-troff-man" },
			{ "me", "application/x-troff-me" },
			{ "ms", "application/x-troff-ms" },
			{ "xml", "application/xml" },
			{ "xsl", "application/xml" },
			{ "xhtml", "application/xhtml+xml" },
			{ "xht", "application/xhtml+xml" },
			{ "dtd", "application/xml-dtd" },
			{ "xslt", "application/xslt+xml" },
			{ "zip", "application/zip" },
			{ "mp3", "audio/mpeg" },
			{ "mpga", "audio/mpeg" },
			{ "mp2", "audio/mpeg" },
			{ "m3u", "audio/x-mpegurl" },
			{ "wav", "audio/x-wav" },
			{ "pdb", "chemical/x-pdb" },
			{ "xyz", "chemical/x-xyz" },
			{ "bmp", "image/bmp" },
			{ "cgm", "image/cgm" },
			{ "gif", "image/gif" },
			{ "ief", "image/ief" },
			{ "jpg", "image/jpeg" },
			{ "jpeg", "image/jpeg" },
			{ "jpe", "image/jpeg" },
			{ "png", "image/png" },
			{ "svg", "image/svg+xml" },
			{ "wbmp", "image/vnd.wap.wbmp" },
			{ "ras", "image/x-cmu-raster" },
			{ "ico", "image/x-icon" },
			{ "pnm", "image/x-portable-anymap" },
			{ "pbm", "image/x-portable-bitmap" },
			{ "pgm", "image/x-portable-graymap" },
			{ "ppm", "image/x-portable-pixmap" },
			{ "rgb", "image/x-rgb" },
			{ "xbm", "image/x-xbitmap" },
			{ "xpm", "image/x-xpixmap" },
			{ "xwd", "image/x-xwindowdump" },
			{ "css", "text/css" },
			{ "html", "text/html" },
			{ "htm", "text/html" },
			{ "txt", "text/plain" },
			{ "asc", "text/plain" },
			{ "rtx", "text/richtext" },
			{ "rtf", "text/rtf" },
			{ "tsv", "text/tab-separated-values" },
			{ "wml", "text/vnd.wap.wml" },
			{ "wmls", "text/vnd.wap.wmlscript" },
			{ "etx", "text/x-setext" },
			{ "mpg", "video/mpeg" },
			{ "mpeg", "video/mpeg" },
			{ "mpe", "video/mpeg" },
			{ "flv", "video/x-flv" },
			{ "avi", "video/x-msvideo" },
			{ "movie", "video/x-sgi-movie" },
		};
	}
    struct Request {
        std::string method;
        std::string path;
        std::string params;
        std::map<std::string, std::string> headers;
        std::map<std::string, std::string> query;
        std::map<std::string, std::string> cookies;
    };
    struct Response {
        Response() {
            code = 200;
            type = "text/html";
            body << "";
        }
		template <class T>
        void send(const T& value) {
           body << value;
        };
        int code;
        std::string type;
        std::stringstream body;
    };
    class SystemError : public std::system_error {
    public:
        SystemError(const char* pMessage,
                    std::error_code err = std::error_code(errno, std::system_category()))
        : std::system_error(err)
        , message(std::string(pMessage) + ": " + err.message()) {}
        const char* what() const throw () { return message.c_str(); }
    private:
        std::string message;
    };
    void list_dir(Request* req, Response* res) {
		static auto mime = create_mime();

        char* base_path = realpath(req->params.c_str(), NULL);
        std::string new_path = "";
        char* actual_path = realpath(req->params.c_str(), NULL);

        auto open_itr = req->query.find("open");
        if (open_itr != req->query.end()) {
            new_path += open_itr->second;
            strcat(actual_path, new_path.c_str());
        }

        if (strncmp(base_path, actual_path, strlen(base_path)) > 0) {
            actual_path = base_path;
        }

		struct stat st_buf;
        if (stat(actual_path, &st_buf))  {
            res->code = 404;
            res->type = "text/plain";
            res->send("Not found");
        } else if (S_ISREG (st_buf.st_mode)) {
            size_t ext_pos = std::string(actual_path).find_last_of(".");
            auto ext_itr = mime.find(std::string(actual_path).substr(ext_pos + 1));
            if (ext_itr != mime.end()) {
                res->type = ext_itr->second;
            } else {
                res->type = "application/octet-stream";
            }

            std::ifstream ifs(actual_path);
            copy(std::istreambuf_iterator<char>(ifs),
                 std::istreambuf_iterator<char>(),
                 std::ostreambuf_iterator<char>(res->body));
        } else if (S_ISDIR (st_buf.st_mode)) {
            DIR* dir_d = opendir(actual_path);
            if (dir_d == NULL)
				throw WPP::SystemError("Unable to open / folder");

            std::stringstream out;
            out << "<title>" << new_path << "</title>" << std::endl;
            out << "<table>";

            while (auto dir = readdir(dir_d)) {
                out << "<tr><td><a href=\"" << req->path << "?open=" << new_path
					<< "/" << dir->d_name << """\">";

                if (dir->d_type == DT_DIR) {
                    out << "[" << dir->d_name << "]";
                } else {
                    out << " " << dir->d_name << "";
                }

                out << "</a></td></tr>";
            }
            out << "</table>";
            res->send(out.str().c_str());
        }
    }
    struct Route {
        std::string path;
        std::string method;
        void (*callback)(Request*, Response*);
        std::string params;
    };
    class Server {
        public:
            void get(std::string, void (*callback)(Request*, Response*));
            void post(std::string, void (*callback)(Request*, Response*));
            void all(std::string, void (*callback)(Request*, Response*));
            void get(std::string, std::string);
            void post(std::string, std::string);
            void all(std::string, std::string);
            bool start(int, std::string);
            bool start(int);
            bool start();
        private:
            void* main_loop(void*);
            void parse_headers(char*, Request*, Response*);
            bool match_route(Request*, Response*);
			std::vector<Route> ROUTES;
	};
    void Server::parse_headers(char* headers, Request* req, Response* res) {
        int i = 0;
        char * pch;
        for (pch = strtok(headers, "\n"); pch; pch = strtok(NULL, "\n" ))
        {
            if (i++ == 0)  {
                std::vector<std::string> R;
                std::string line(pch);
                split(line, " ", 3, &R);

                if (R.size() != 3) {
    //                throw error
                }

                req->method = R[0];
                req->path = R[1];

                size_t pos = req->path.find('?');
                
                // We have GET params here
                if (pos != std::string::npos)  {
                    std::vector<std::string> Q1;
                    split(req->path.substr(pos + 1), "&", -1, &Q1);

                    for (std::vector<std::string>::size_type q = 0; q < Q1.size(); q++) {
                        std::vector<std::string> Q2;
                        split(Q1[q], "=", -1, &Q2);

                        if (Q2.size() == 2) {
                            req->query[Q2[0]] = Q2[1];
                        }
                    }

                    req->path = req->path.substr(0, pos);
                }
            } else {
                std::vector<std::string> R;
                std::string line(pch);
                split(line, ": ", 2, &R);

                if (R.size() == 2) {
                    req->headers[R[0]] = R[1];
                    
                    // Yeah, cookies!
                    if (R[0] == "Cookie") {
                        std::vector<std::string> C1;
                        split(R[1], "; ", -1, &C1);
                        
                        for (std::vector<std::string>::size_type c = 0; c < C1.size(); c++) {
                            std::vector<std::string> C2;
                            split(C1[c], "=", 2, &C2);
                            
                            req->cookies[C2[0]] = C2[1];
                        }
                    }
                }
            }
        }
    }

    void Server::get(std::string path, void (*callback)(Request*, Response*)) {
        Route r = {
             path,
             "GET",
             callback
        };

        ROUTES.push_back(r);
    }

    void Server::post(std::string path, void (*callback)(Request*, Response*)) {
        Route r = {
             path,
             "POST",
             callback
        };

        ROUTES.push_back(r);
    }

    void Server::all(std::string path, void (*callback)(Request*, Response*)) {
        Route r = {
             path,
             "ALL",
             callback
        };

        ROUTES.push_back(r);
    }

    void Server::get(std::string path, std::string loc) {
        Route r = {
             path,
             "GET",
             &list_dir,
             loc
        };

        ROUTES.push_back(r);
    }

    void Server::post(std::string path, std::string loc) {
        Route r = {
             path,
             "POST",
             &list_dir,
             loc
        };

        ROUTES.push_back(r);
    }

    void Server::all(std::string path, std::string loc) {
        Route r = {
             path,
             "ALL",
             &list_dir,
             loc
        };

        ROUTES.push_back(r);
    }

    bool Server::match_route(Request* req, Response* res) {
        for (std::vector<Route>::size_type i = 0; i < ROUTES.size(); i++) {
            if (ROUTES[i].path == req->path &&
				(ROUTES[i].method == req->method ||
				 ROUTES[i].method == "ALL")) {

                req->params = ROUTES[i].params;
                ROUTES[i].callback(req, res);
                return true;
            }
        }

        return false;
    }

    void* Server::main_loop(void* arg) {
        int* port = reinterpret_cast<int*>(arg);

        int newsc;

        int sc = socket(AF_INET, SOCK_STREAM, 0);

        if (sc < 0) {
            throw WPP::SystemError("ERROR opening socket");
        }

        struct sockaddr_in serv_addr, cli_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(*port);

        if (::bind(sc, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            throw WPP::SystemError("ERROR on binding");
        }

        listen(sc, 5);

        socklen_t clilen = sizeof(cli_addr);

        while (true) {
            newsc = accept(sc, (struct sockaddr *) &cli_addr, &clilen);

            if (newsc < 0) {
                throw WPP::SystemError("ERROR on accept");
            }

            // handle new connection
            Request req;
            Response res;

            static char headers[BUFSIZE + 1];
            long ret = read(newsc, headers, BUFSIZE);
            if (ret > 0 && ret < BUFSIZE) {
                headers[ret] = 0;
            } else {
                headers[0] = 0;
            }

            this->parse_headers(headers, &req, &res);

            if (!this->match_route(&req, &res)) {
                res.code = 404;
                res.type = "text/plain";
                res.send("Not found");
            }

            char header_buffer[BUFSIZE];
            const char* body = res.body.str().c_str();
            size_t body_len = strlen(body);

            // build http response
            sprintf(header_buffer, "HTTP/1.0 %d\r\n", res.code);

            // append headers
            sprintf(&header_buffer[strlen(header_buffer)], "Server: %s %s\r\n", SERVER_NAME, SERVER_VERSION);
            sprintf(&header_buffer[strlen(header_buffer)], "Content-Type: %s\r\n", res.type.c_str());
            sprintf(&header_buffer[strlen(header_buffer)], "Content-Length: %zd\r\n", body_len);

            // append extra crlf to indicate start of body
            strcat(header_buffer, "\r\n");

            ssize_t t;
            t = write(newsc, header_buffer, strlen(header_buffer));
            t = write(newsc, body, body_len);
        }
    }

    bool Server::start(int port, std::string host) {
        this->main_loop(&port);
        return true;
    }

    bool Server::start(int port) {
         return this->start(port, "0.0.0.0");
    }

    bool Server::start() {
         return this->start(80);
    }
}
