#include<iostream>

/*
    Strategy_pattern ,当某一个类面对不同的情况需要做出不同情况的行为时，使用策略模式
    出行类
*/
//class Abstract_Behavior;
//class Bike;
//class Subway;


class Abstract_Behavior{  //抽象行为类
public:
    Abstract_Behavior(/* args */) = default;
    virtual ~Abstract_Behavior() = default;

    virtual void How_to_go() = 0;
};


class Bike : public Abstract_Behavior{
private:
    /* data */
public:
    Bike(/* args */) = default;
    ~Bike() = default;

    virtual void How_to_go() override {
        std::cout<<"choose bike go out!"<<std::endl;
    }
};


class Subway : public Abstract_Behavior{
public:
    Subway(/* args */) = default;
    ~Subway() = default;

    virtual void How_to_go () override {
        std::cout<<"choose subway go out!"<<std::endl;
    }
};


class Trip{
private:
    Abstract_Behavior* Strategy;
public:

    enum class Type:char { Bike, Subway };  //限定作用域的强枚举类型
    Trip(/* args */) : Strategy(nullptr) {};
    ~Trip() {
        if(!Strategy)
            delete Strategy;
    };
 
    void Go_out(Type type) {   //应对不同的情况来选择Strategy
        if( !this->Strategy ){
            delete this->Strategy;
            this->Strategy = nullptr;
        }
        
        switch (type)
        {
        case Type::Bike :
            this->Strategy = new Bike();
            break;
        case Type::Subway :
            this->Strategy = new Subway();
            break;
        default:
            break;
        }

        if(this->Strategy != nullptr)
            this->Strategy->How_to_go();
    }
};


int main() {
    //go out
    Trip* obj = new Trip();
    obj->Go_out(Trip::Type::Bike);
    obj->Go_out(Trip::Type::Subway);

    delete obj;
}







