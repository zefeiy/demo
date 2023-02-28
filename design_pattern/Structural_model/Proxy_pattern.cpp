#include<iostream>

/*
    Proxy Pattern  ,为其他对象提供代理，控制对象访问,代理与对象之间是一个关联关系
    本来可以直接访问对象，但通过创建代理对象来间接的访问对象
*/

class Abstract_com{
public:
    Abstract_com(/* args */) = default;
    virtual ~Abstract_com() = default;

    virtual void Communication() = 0;
};


class obj : public Abstract_com{
public:
    obj(/* args */) = default;
    ~obj() = default;

    void Communication() override {
        std::cout<<"communication"<<std::endl;
    }
};

class Proxy_obj : public Abstract_com{
private:
    obj* ProxyObj;
public:
    Proxy_obj() {
        if(ProxyObj == nullptr)
            this->ProxyObj = new obj();
    };
    ~Proxy_obj() {
        if(this->ProxyObj != nullptr)
            delete this->ProxyObj;
    };  

    void Communication() override {
        std::cout<<"Proxy"<<std::endl;
        ProxyObj->Communication();
    }
};



int main () {

    //1.Access obj ->through direct access
    Abstract_com* o = new obj();
    o->Communication();
    delete o;
    //2. Through proxy indirect access
    Abstract_com* p = new Proxy_obj();
    p->Communication();
    delete p;

    return 0;
}