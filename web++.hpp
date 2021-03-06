#include <dirent.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#define SERVER_NAME "Web++"
#define SERVER_VERSION "1.0.1"

#define BUFSIZE 8096

using namespace std;

namespace WPP {
    class Request {
        public:
            Request() {

            }
            std::string method;
            std::string path;
            std::string params;
            map<string, string> headers;
            map<string, string> query;
            map<string, string> cookies;

        private:

    };

    class Response {
        public:
            Response() {
                code = 200;
                type = "text/html";
                body << "";
            }
            int code;
            string type;
            stringstream body;
            void send(string str) {
               body << str;
            };
            void send(const char* str) {
               body << str;
            };
        private:
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

    map<string, string> mime;

    void list_dir(Request* req, Response* res) {
        unsigned char isFile = 0x8, isFolder = 0x4;
        struct dirent *dir;
        int status;
        struct stat st_buf;

        // Mime
        mime["atom"] = "application/atom+xml";
        mime["hqx"] = "application/mac-binhex40";
        mime["cpt"] = "application/mac-compactpro";
        mime["mathml"] = "application/mathml+xml";
        mime["doc"] = "application/msword";
        mime["bin"] = "application/octet-stream";
        mime["dms"] = "application/octet-stream";
        mime["lha"] = "application/octet-stream";
        mime["lzh"] = "application/octet-stream";
        mime["exe"] = "application/octet-stream";
        mime["class"] = "application/octet-stream";
        mime["so"] = "application/octet-stream";
        mime["dll"] = "application/octet-stream";
        mime["dmg"] = "application/octet-stream";
        mime["oda"] = "application/oda";
        mime["ogg"] = "application/ogg";
        mime["pdf"] = "application/pdf";
        mime["ai"] = "application/postscript";
        mime["eps"] = "application/postscript";
        mime["ps"] = "application/postscript";
        mime["rdf"] = "application/rdf+xml";
        mime["smi"] = "application/smil";
        mime["smil"] = "application/smil";
        mime["gram"] = "application/srgs";
        mime["grxml"] = "application/srgs+xml";
        mime["air"] = "application/vnd.adobe.apollo-application-installer-package+zip";
        mime["mif"] = "application/vnd.mif";
        mime["xul"] = "application/vnd.mozilla.xul+xml";
        mime["xls"] = "application/vnd.ms-excel";
        mime["ppt"] = "application/vnd.ms-powerpoint";
        mime["rm"] = "application/vnd.rn-realmedia";
        mime["wbxml"] = "application/vnd.wap.wbxml";
        mime["wmlc"] = "application/vnd.wap.wmlc";
        mime["wmlsc"] = "application/vnd.wap.wmlscriptc";
        mime["vxml"] = "application/voicexml+xml";
        mime["bcpio"] = "application/x-bcpio";
        mime["vcd"] = "application/x-cdlink";
        mime["pgn"] = "application/x-chess-pgn";
        mime["cpio"] = "application/x-cpio";
        mime["csh"] = "application/x-csh";
        mime["dcr"] = "application/x-director";
        mime["dir"] = "application/x-director";
        mime["dxr"] = "application/x-director";
        mime["dvi"] = "application/x-dvi";
        mime["spl"] = "application/x-futuresplash";
        mime["gtar"] = "application/x-gtar";
        mime["hdf"] = "application/x-hdf";
        mime["js"] = "application/x-javascript";
        mime["latex"] = "application/x-latex";
        mime["sh"] = "application/x-sh";
        mime["shar"] = "application/x-shar";
        mime["swf"] = "application/x-shockwave-flash";
        mime["sit"] = "application/x-stuffit";
        mime["sv4cpio"] = "application/x-sv4cpio";
        mime["sv4crc"] = "application/x-sv4crc";
        mime["tar"] = "application/x-tar";
        mime["tcl"] = "application/x-tcl";
        mime["tex"] = "application/x-tex";
        mime["man"] = "application/x-troff-man";
        mime["me"] = "application/x-troff-me";
        mime["ms"] = "application/x-troff-ms";
        mime["xml"] = "application/xml";
        mime["xsl"] = "application/xml";
        mime["xhtml"] = "application/xhtml+xml";
        mime["xht"] = "application/xhtml+xml";
        mime["dtd"] = "application/xml-dtd";
        mime["xslt"] = "application/xslt+xml";
        mime["zip"] = "application/zip";
        mime["mp3"] = "audio/mpeg";
        mime["mpga"] = "audio/mpeg";
        mime["mp2"] = "audio/mpeg";
        mime["m3u"] = "audio/x-mpegurl";
        mime["wav"] = "audio/x-wav";
        mime["pdb"] = "chemical/x-pdb";
        mime["xyz"] = "chemical/x-xyz";
        mime["bmp"] = "image/bmp";
        mime["cgm"] = "image/cgm";
        mime["gif"] = "image/gif";
        mime["ief"] = "image/ief";
        mime["jpg"] = "image/jpeg";
        mime["jpeg"] = "image/jpeg";
        mime["jpe"] = "image/jpeg";
        mime["png"] = "image/png";
        mime["svg"] = "image/svg+xml";
        mime["wbmp"] = "image/vnd.wap.wbmp";
        mime["ras"] = "image/x-cmu-raster";
        mime["ico"] = "image/x-icon";
        mime["pnm"] = "image/x-portable-anymap";
        mime["pbm"] = "image/x-portable-bitmap";
        mime["pgm"] = "image/x-portable-graymap";
        mime["ppm"] = "image/x-portable-pixmap";
        mime["rgb"] = "image/x-rgb";
        mime["xbm"] = "image/x-xbitmap";
        mime["xpm"] = "image/x-xpixmap";
        mime["xwd"] = "image/x-xwindowdump";
        mime["css"] = "text/css";
        mime["html"] = "text/html";
        mime["htm"] = "text/html";
        mime["txt"] = "text/plain";
        mime["asc"] = "text/plain";
        mime["rtx"] = "text/richtext";
        mime["rtf"] = "text/rtf";
        mime["tsv"] = "text/tab-separated-values";
        mime["wml"] = "text/vnd.wap.wml";
        mime["wmls"] = "text/vnd.wap.wmlscript";
        mime["etx"] = "text/x-setext";
        mime["mpg"] = "video/mpeg";
        mime["mpeg"] = "video/mpeg";
        mime["mpe"] = "video/mpeg";
        mime["flv"] = "video/x-flv";
        mime["avi"] = "video/x-msvideo";
        mime["movie"] = "video/x-sgi-movie";

        char* base_path = realpath(req->params.c_str(), NULL);
        string new_path = "";
        char* actual_path = realpath(req->params.c_str(), NULL);

        auto open_itr = req->query.find("open");
        if(open_itr != req->query.end()) {
            new_path += open_itr->second;
            strcat(actual_path, new_path.c_str());
        }

        if(strncmp(base_path, actual_path, strlen(base_path)) > 0) {
            actual_path = base_path;
        }

        status = stat(actual_path, &st_buf);

        if (status != 0)  {
            res->code = 404;
            res->type = "text/plain";
            res->send("Not found");
        } else if (S_ISREG (st_buf.st_mode)) {
            size_t ext_pos = string(actual_path).find_last_of(".");

            map<string, string>::iterator ext = mime.find(string(actual_path).substr(ext_pos + 1));

            if(ext != mime.end()) {
                res->type = ext->second;
            } else {
                res->type = "application/octet-stream";
            }

            ifstream ifs(actual_path);

            copy(istreambuf_iterator<char>(ifs),
                 istreambuf_iterator<char>(),
                 ostreambuf_iterator<char>(res->body));
        } else if (S_ISDIR (st_buf.st_mode)) {
            DIR* dir_d = opendir(actual_path);

            if (dir_d == NULL) throw WPP::SystemError("Unable to open / folder");

            std::stringstream out;
            out << "<title>" << new_path << "</title>" << endl;
            out << "<table>";

            while((dir = readdir(dir_d))) {
                out << "<tr><td><a href=\"" << req->path << "?open=" << new_path << "/" << dir->d_name << """\">";

                if (dir->d_type == isFolder) {
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
        string path;
        string method;
        void (*callback)(Request*, Response*);
        string params;
    };

    std::vector<Route> ROUTES;

    class Server {
        public:
            void get(string, void (*callback)(Request*, Response*));
            void post(string, void (*callback)(Request*, Response*));
            void all(string, void (*callback)(Request*, Response*));
            void get(string, string);
            void post(string, string);
            void all(string, string);
            bool start(int, string);
            bool start(int);
            bool start();
        private:
            void* main_loop(void*);
            void parse_headers(char*, Request*, Response*);
            bool match_route(Request*, Response*);
            string trim(string);
            void split(string, string, int, vector<string>*);
    };

    void Server::split(string str, string separator, int max, vector<string>* results){
        int i = 0;
        size_t found = str.find_first_of(separator);

        while(found != string::npos){
            if(found > 0){
                results->push_back(str.substr(0, found));
            }
            str = str.substr(found+1);
            found = str.find_first_of(separator);

            if(max > -1 && ++i == max) break;
        }

        if(str.length() > 0){
            results->push_back(str);
        }
    }

    string Server::trim(string s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());

        return s;
    }

    void Server::parse_headers(char* headers, Request* req, Response* res) {
        // Parse request headers
        int i = 0;
        char * pch;
        for (pch = strtok(headers, "\n"); pch; pch = strtok(NULL, "\n" ))
        {
            if(i++ == 0)  {
                vector<string> R;
                string line(pch);
                this->split(line, " ", 3, &R);

    //            cout << R.size() << endl;

                if(R.size() != 3) {
    //                throw error
                }

                req->method = R[0];
                req->path = R[1];

                size_t pos = req->path.find('?');
                
                // We have GET params here
                if(pos != string::npos)  {
                    vector<string> Q1;
                    this->split(req->path.substr(pos + 1), "&", -1, &Q1);

                    for(vector<string>::size_type q = 0; q < Q1.size(); q++) {
                        vector<string> Q2;
                        this->split(Q1[q], "=", -1, &Q2);

                        if(Q2.size() == 2) {
                            req->query[Q2[0]] = Q2[1];
                        }
                    }

                    req->path = req->path.substr(0, pos);
                }
            } else {
                vector<string> R;
                string line(pch);
                this->split(line, ": ", 2, &R);

                if(R.size() == 2) {
                    req->headers[R[0]] = R[1];
                    
                    // Yeah, cookies!
                    if(R[0] == "Cookie") {
                        vector<string> C1;
                        this->split(R[1], "; ", -1, &C1);
                        
                        for(vector<string>::size_type c = 0; c < C1.size(); c++) {
                            vector<string> C2;
                            this->split(C1[c], "=", 2, &C2);
                            
                            req->cookies[C2[0]] = C2[1];
                        }
                    }
                }
            }
        }
    }

    void Server::get(string path, void (*callback)(Request*, Response*)) {
        Route r = {
             path,
             "GET",
             callback
        };

        ROUTES.push_back(r);
    }

    void Server::post(string path, void (*callback)(Request*, Response*)) {
        Route r = {
             path,
             "POST",
             callback
        };

        ROUTES.push_back(r);
    }

    void Server::all(string path, void (*callback)(Request*, Response*)) {
        Route r = {
             path,
             "ALL",
             callback
        };

        ROUTES.push_back(r);
    }

    void Server::get(string path, string loc) {
        Route r = {
             path,
             "GET",
             &list_dir,
             loc
        };

        ROUTES.push_back(r);
    }

    void Server::post(string path, string loc) {
        Route r = {
             path,
             "POST",
             &list_dir,
             loc
        };

        ROUTES.push_back(r);
    }

    void Server::all(string path, string loc) {
        Route r = {
             path,
             "ALL",
             &list_dir,
             loc
        };

        ROUTES.push_back(r);
    }

    bool Server::match_route(Request* req, Response* res) {
        for (vector<Route>::size_type i = 0; i < ROUTES.size(); i++) {
            if(ROUTES[i].path == req->path && (ROUTES[i].method == req->method || ROUTES[i].method == "ALL")) {
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

        socklen_t clilen;
        clilen = sizeof(cli_addr);

        while(true) {
            newsc = accept(sc, (struct sockaddr *) &cli_addr, &clilen);

            if (newsc < 0) {
                throw WPP::SystemError("ERROR on accept");
            }

            // handle new connection
            Request req;
            Response res;

            static char headers[BUFSIZE + 1];
            long ret = read(newsc, headers, BUFSIZE);
            if(ret > 0 && ret < BUFSIZE) {
                headers[ret] = 0;
            } else {
                headers[0] = 0;
            }

            this->parse_headers(headers, &req, &res);

            if(!this->match_route(&req, &res)) {
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

    bool Server::start(int port, string host) {
//         pthread_t worker;

//         for(int i = 0; i < 1; ++i) {
//              int rc = pthread_create (&worker, NULL, &mainLoop, NULL);
//              assert (rc == 0);
//         }

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
