#include<iostream>
#include<memory>
/*  Decorative pattern 不改变对象原有结构的基础上，给对象添加新的功能
*/

//抽象基础构件类
class Transfrom{
public:
    Transfrom(/* args */) = default;
    virtual ~Transfrom() = default;
    virtual void move() = 0;
};

class Car : public Transfrom{  //基础构件类
private:
    /* data */
public:
    Car(/* args */) = default;
    ~Car() = default;

    virtual void move() override {
        std::cout<<"汽车在陆地开动"<<std::endl;
    }
};

//抽象装饰类，其子类赋予基础构件类更多功能
class AbstractDecorative : public Transfrom{  
private:
    std::shared_ptr<Transfrom> transfrom;
    //Transfrom* transfrom;  
public:
    AbstractDecorative() = default;

    AbstractDecorative(std::shared_ptr<Transfrom> obj) : transfrom(obj){}

    virtual ~AbstractDecorative(){
        std::cout<<"释放被装饰的实例"<<std::endl;    
    }
};

class Robot : public AbstractDecorative{  
private:
    /* data */
public:
    Robot() = default;
    Robot(std::shared_ptr<Transfrom> obj) :AbstractDecorative(obj) {
        std::cout<<"已装饰机器人智能化功能..."<<std::endl;
    }
    ~Robot() = default;

    virtual void move() override {
        std::cout<<"机器人移动"<<std::endl;
    }

    void say(){
        std::cout<<"添加了机器人智能化功能，说话"<<std::endl;
    }
};


class Super_robot : public AbstractDecorative{
private:
    /* data */
public:
    Super_robot(std::shared_ptr<Transfrom> obj) : AbstractDecorative(obj){
        std::cout<<"加装超级机器人系统"<<std::endl;
    }
    ~Super_robot() = default;

    virtual void move() {
        std::cout<<"超级机器人飞行移动"<<std::endl;
    }

    void attack(){
        std::cout<<"attack system start!"<<std::endl;
    }

};

int main(){
    //1.实体汽车类 
    std::shared_ptr<Transfrom> car = std::make_shared<Car>();
    car->move();
    //2.给汽车添加机器人智能功能
    std::shared_ptr<Robot> robot = std::make_shared<Robot>(car);
    robot->move();
    robot->say();
    //delete robot;
    //3.再次装饰
    std::shared_ptr<Super_robot> rot = std::make_shared<Super_robot>(robot);
    //Super_robot* rot = new Super_robot(robot);
    rot->move();
    rot->attack();

    return 0;
}











