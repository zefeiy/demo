#include<iostream>
#include<string>
#include<list>
/*  
Observer_pattern 当⼀个对象的改变需要同时改变其他对象，且不知道具体有多少对象有待改变时，应该考虑使⽤观察者模式；
⼀个抽象模型有两个⽅⾯，其中⼀⽅⾯依赖于另⼀⽅⾯，这时可以⽤观察者模式将这两者封装在独⽴的对象中使它
们各⾃独⽴地改变和复⽤
*/

class Abstract_observer; 

class Subject{   //被观察对象,功能： 1. 有变化是通知观察者  2. 添加观察者，删除观察者，
public:
    Subject(/* args */) = default;
    virtual ~Subject() = default;

    void attach(Abstract_observer* obj){
        List.emplace_back(obj);
    }

    void deatch(Abstract_observer* obj){
        std::list<Abstract_observer*>::iterator iter = List.begin();
        while(iter != List.end()){
            if(*iter == obj){
                List.erase(iter);
                return;
            }
            ++iter;
        }
    }

    virtual void notify() = 0;

protected:
    std::list<Abstract_observer*> List;
};

                        
class Abstract_observer{  //抽象观察者基类,可以实例化不同的观察者
protected:
    Subject* _sub;
    std::string _name;
public:
    Abstract_observer(Subject* sub, std::string name) : _sub(sub), _name(name){
        _sub->attach(this);
    }

    void unsubscribe(){
        _sub->deatch(this);
    }

    virtual ~Abstract_observer() = default;

    virtual void update() = 0;  //被通知后观察者更新自身状态函数
};


class Alarm : public Subject{  //放在这里是为了可以观察者和被观察者之间的相互引用

public:
    Alarm(/* args */) = default;
    ~Alarm() = default;

    void notify() override {
        std::cout<<"开始通知, 到下班时间了"<<std::endl;
        
        std::list<Abstract_observer*>::iterator iter = this->List.begin();
        while(iter != List.end() ){
            (*iter)->update();
            ++iter;
        }
    }
};



class Observer_boss : public Abstract_observer{
public:
    Observer_boss(Subject* sub, std::string name) : Abstract_observer(sub, name){
    };
    ~Observer_boss() = default;

    void update() override {
        std::cout<<"下班了: 我是老板，所以我回家了吃饭了"<<std::endl;
    }
};


class Observer_staff : public Abstract_observer{
private:
    /* data */
public:
    Observer_staff(Subject* sub, std::string name) : Abstract_observer(sub, name){}
    ~Observer_staff() = default;

    void update() override {
        std::cout<<"下班了: 我是员工，所以我还要加班......"<<std::endl;
    }
};


int main(){
    //1.创建被观察者
    Subject* sub = new Alarm();
    //2. 创建观察者, 传递观察者对象
    Abstract_observer* boss = new Observer_boss(sub, "老板");
    Abstract_observer* staff = new Observer_staff(sub, "员工");
    //3.被观察者通知  观察者
    std::cout<<"下班时间到, 被观察者闹钟通知所有公司人员.."<<std::endl;
    sub->notify();
    std::cout<<"第二天.."<<std::endl;
    std::cout<<"老板良心发现, 不再每天盯着时钟看了, 取消对时钟的观察"<<std::endl;
    boss->unsubscribe();
    std::cout<<"下班时间到, 被观察者闹钟通知所有公司人员.."<<std::endl;
    sub->notify();

    return 0;
}