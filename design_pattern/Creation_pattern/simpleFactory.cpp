#include<iostream>

/*
    simpleFactory pattern
    简单工厂模式，工厂类有且仅有一个，工厂类依赖抽象产品父类进而生成产品子类实例
*/

 
class AbstractSmile{  //抽象产品父类

public:

    virtual void transfrom() = 0;

    virtual void ability() = 0;

    AbstractSmile(){};
    
    virtual ~AbstractSmile(){};
};

class SheepSmile : public AbstractSmile{

public:
    SheepSmile();
    ~SheepSmile();

    virtual void transfrom() override{
        std::cout<<"山羊变换"<<std::endl;
    }

    virtual void ability() override {
        std::cout<<"山羊ability"<<std::endl;
    }

};
SheepSmile::SheepSmile(){}
SheepSmile::~SheepSmile(){}

class Bat : public AbstractSmile{

public:
    Bat();
    ~Bat();

    virtual void transfrom() override {
        std::cout<<"蝙蝠变换"<<std::endl;
    }

    virtual void ability() override {
        std::cout<<"蝙蝠ability"<<std::endl;
    }
};
Bat::Bat(){}
Bat::~Bat(){}


class SimpleFactory{
private:
    
public:

    enum class Type:char { Sheep, Bat };   //限定作用域范围的强类型枚举 去除class则(不限定作用域的强类型枚举)

    AbstractSmile* Create_smile(Type type){
        AbstractSmile* ptr = nullptr;
        
        switch (type){
        case Type::Sheep :
            ptr = new SheepSmile();
            break;
        case Type::Bat :
            ptr = new Bat();
            break;
        default:
            break;
        }
        return ptr;
    }

    SimpleFactory(/* args */);
    ~SimpleFactory();
};

SimpleFactory::SimpleFactory(/* args */){}

SimpleFactory::~SimpleFactory(){}


int main(){

    SimpleFactory* Factory = new SimpleFactory();
    AbstractSmile* Sheep = Factory->Create_smile(SimpleFactory::Type::Sheep);
    AbstractSmile* Bat = Factory->Create_smile(SimpleFactory::Type::Bat);

    Sheep->transfrom();
    Sheep->ability();
    Bat->transfrom();
    Bat->ability();

    return 0;
}



