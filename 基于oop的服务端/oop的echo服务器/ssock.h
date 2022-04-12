#include"Addrinit.h"
class Socket{
private: 
    int m_fd;

public:
    Socket();
    Socket(int fd);
    ~Socket();
    
    int getfd();

    void s_bind(Addrinit*);

    void s_listen();

    int s_accept(Addrinit*); 


};