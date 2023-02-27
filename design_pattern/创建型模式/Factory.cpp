#include<iostream>
/*
    Factory pattern
    工厂模式，工厂类有n个对应N个产品，将工厂与产品进行解耦合，工厂类继承抽象工厂类，抽象工厂类依赖抽象产品父类进而生成产品子类实例,  工厂 N ：N  产品

    工厂类符合开发封闭原则，   简单工厂模式不符合开发封闭原则（创建新的对象需要修改工厂类）
*/
 
class AbstractSmile{  //抽象产品父类

public:

    virtual void transfrom() = 0;

    virtual void ability() = 0;

    AbstractSmile() = default;
    
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


class AbstractFactory{

public:
    AbstractFactory() = default;
    virtual ~AbstractFactory(){};
    virtual AbstractSmile* Create_simle() = 0;
};


//在此基础上创建新的工厂来创建新的对象
class BatFactory : public AbstractFactory{
public:
    BatFactory() = default;
    ~BatFactory(){};

    virtual AbstractSmile* Create_simle(){
        return new Bat();
    }

};


class SheepFactory : public AbstractFactory{
public:
    SheepFactory() = default;
    ~SheepFactory(){};

    virtual AbstractSmile* Create_simle(){
        return new SheepSmile();
    }
};


int main(){

    //需要一个Sheepsimle对象
    AbstractFactory* Factory = new SheepFactory();
    AbstractSmile* sheep = Factory->Create_simle();

    sheep->ability();
    sheep->transfrom();

    delete Factory;
    delete sheep;

    return 0;
}