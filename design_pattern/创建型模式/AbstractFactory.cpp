#include<string>
#include<iostream>

/*
    抽象工厂模式 ->>>> 在工厂模式的基础上，应对创建复杂对象类型使用 
*/

class ShipBody{  //船体抽象类
public:
    ShipBody() = default;
    virtual ~ShipBody() = default;
    virtual std::string getShipBody() = 0;
};

//船体子类 分类 ->>多中船体
class WoodBody : public ShipBody{
public:
    WoodBody () = default;
    ~WoodBody () = default;

    virtual std::string getShipBody() override {
        return std::string("使用<木材> 制作轮船船体..");
    }
};

class IronBody : public ShipBody{
public:
    IronBody () = default;
    ~IronBody () = default;

    virtual std::string getShipBody() override {
        return std::string("使用<钢铁> 制作轮船船体..");
    }
};

class MetalBody : public ShipBody{
public:
    MetalBody () = default;
    ~MetalBody () = default;

    virtual std::string getShipBody() override {
        return std::string("使用<合金> 制作轮船船体..");
    }
};

//引擎抽象类
class Engine{
public:
    Engine() = default;
    virtual ~Engine() = default;
    virtual std::string getShipEngine() = 0;
};

class Human : public Engine{
public:
    Human() = default;
    ~Human() = default;
    std::string getShipEngine() override {
        return std::string("使用<人力> 驱动..");
    }
};

class Diesel : public Engine{
public:
    Diesel() = default;
    ~Diesel() = default;

    std::string getShipEngine() override {
        return std::string("使用<内燃机> 驱动..");
    }
};

class Nuclear : public Engine{
public:
    Nuclear() = default;
    ~Nuclear() = default;

    std::string getShipEngine() override {
        return std::string("使用<核能> 驱动..");
    }
};

//武器抽象类
class Weapon{
public:
    Weapon(/* args */) = default;
    virtual ~Weapon() = default;
    virtual std::string getShipWeapon() = 0;
};

class Gun : public Weapon{
public:
    Gun(/* args */) = default;
    ~Gun() = default;
    virtual std::string getShipWeapon() override {
        return std::string("配备的武器是<枪>");
    }
};

class Cannon : public Weapon{
public:
    Cannon(/* args */) = default;
    ~Cannon() = default;
    virtual std::string getShipWeapon() override {
        return std::string("配备的武器是<自动机关炮>");
    }
};

class Laser : public Weapon{
public:
    Laser(/* args */) = default;
    ~Laser() = default;
    virtual std::string getShipWeapon() override {
        return std::string("配备的武器是<激光>");
    }
};

//实体船类 ->> 所有的抽象零件组合后便是一艘船了 ->> 使用组合
class Ship{
private:
    ShipBody* _shipbody = nullptr;
    Engine* _engine = nullptr;
    Weapon* _weapon = nullptr;
public:
    Ship() = default;
    Ship(ShipBody* shipBody, Engine* engine, Weapon* weapon) :
    _shipbody(shipBody), _engine(engine), _weapon(weapon){};
    ~Ship(){
        delete _shipbody;
        delete _engine;
        delete _weapon;
    }

    std::string getProperty(){
        std::string info = _shipbody->getShipBody() + _engine->getShipEngine() + _weapon->getShipWeapon();
        return info;
    }

};

//抽象工厂类,提供造船接口
class AbstractFactory{
public:
    AbstractFactory(/* args */) = default;
    virtual ~AbstractFactory() = default;
    virtual Ship* CreateShip() = 0;
};

class BasicFactory : public AbstractFactory{
public:
    BasicFactory(/* args */) = default;
    ~BasicFactory() = default;

    Ship* CreateShip() override {
        Ship *ptr = new Ship(new WoodBody(), new Human(), new Gun());
        std::cout<<"基础船创造完成!"<<std::endl;
        return ptr;
    }
};

class StandardFactory : public AbstractFactory{
public:
    StandardFactory(/* args */) = default;
    ~StandardFactory() = default;

    Ship* CreateShip() override {
        Ship *ptr = new Ship(new IronBody(), new Diesel(), new Cannon());
        std::cout<<"标准船创造完成!"<<std::endl;
        return ptr;
    }
};

class UltimateFactory : public AbstractFactory{
public:
    UltimateFactory(/* args */) = default;
    ~UltimateFactory() = default;

    Ship* CreateShip() override {
        Ship *ptr = new Ship(new MetalBody, new Nuclear(), new Laser());
        std::cout<<"高级船创造完成!"<<std::endl;
        return ptr;
    }
};


int main(){

    //我需要一艘标准船
    AbstractFactory* Factory = new StandardFactory();
    Ship* ship = Factory->CreateShip();
    std::cout<<ship->getProperty()<<std::endl;

    return 0;
}


