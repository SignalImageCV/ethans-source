#include <iostream>
#include <ros/ros.h>
#include <std_msgs/Float32.h>
using namespace std;
class kalman_filter
 {
public:
      kalman_filter(float q,float r)
    {
        Q=q;
        R=r;//如果观测噪声大，我们就不太相信它，将R设大，告诉滤波器不太相信观测值，更相信预测值。
        A=1.0;
        H=1.0;
        pub = nh.advertise<std_msgs::Float32>("/filtered", 1);
        sub = nh.subscribe("/gauss_noise",3,&kalman_filter::call_back,this);
    }

    void call_back(const std_msgs::Float32::ConstPtr measure)
        {
            ros::param::get("Q_noise",Q);
            ros::param::get("R_noise",R);
            //prediction
            //x_raw = A*x_last;
            x_raw = A*x;
            //p_raw=A*p_last*A+Q;
            p_raw=A*p*A+Q;
            cout<<"Q is :"<<Q<<endl;
            cout<<"R is :"<<R<<endl;
            //measurment
            K=p_raw*H/(H*p_raw*H+R);
            x=x_raw+K*((*measure).data-H*x_raw);
            p=(1-K*H)*p_raw;
            //update
            //x_last=x;
            //p_last=p;
            //pub
            X.data=x;
            pub.publish(X);
        }



private:
    float x;
    float x_raw;
    //float x_last;
    float p;
    float p_raw;
    //float p_last;
    float R;
    float Q;
    float K;
    float A;
    float H;
    ros::NodeHandle nh;
    ros::Publisher pub;
    ros::Subscriber sub;
    std_msgs::Float32 X;
 };

int main(int argc,char** argv) {
    cout<<"Lets start kalmanfilter!\n"<<endl;
    ros::init(argc,argv,"kalmanfilter");
    kalman_filter kalman_filter1(0.1,0.5);
    ros::spin();
}