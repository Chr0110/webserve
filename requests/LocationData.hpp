
#ifndef LOCATIONDATA_HPP
#define LOCATIONDATA_HPP

#include <string>

namespace ws
{

	class LocationData {
		public :
			typedef	std::string			string;

			LocationData() : root(""), defaultPage(""), autoindex(false), cgi(false), cgiGet(false), cgiPost(false), cgiDelete(false) {};
			~LocationData() {};

			// SETTERS
			void	setRoot(string root) { this->root = root;};
			void	setDefaultPage(string defaultPage) { this->defaultPage = defaultPage;};
			void	setAutoindex(bool autoindex) { this->autoindex = autoindex;};
			void	setCgi(bool cgi) { this->cgi = cgi;};
			void	setCgiGet(bool cgiGet) { this->cgiGet = cgiGet;};
			void	setCgiPost(bool cgiPost) { this->cgiPost = cgiPost;};
			void	setCgiDelete(bool cgiDelete) { this->cgiDelete = cgiDelete;};

			// GETTERS
			string	getRoot() const { return (this->root);};
			string	getDefaultPage() const { return (this->defaultPage);};
			bool	getAutoindex() const { return (this->autoindex);};
			bool	getCgi() const { return (this->cgi);};
			bool	getCgiGet() const { return (this->cgiGet);};
			bool	getCgiPost() const { return (this->cgiPost);};
			bool	getCgiDelete() const { return (this->cgiDelete);};

		private :
			string		root;
			string		defaultPage;
			bool		autoindex;
			bool		cgi;
			bool		cgiGet;
			bool		cgiPost;
			bool		cgiDelete;
	};

}

#endif