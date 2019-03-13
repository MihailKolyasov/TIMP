#include <iostream>
using namespace std;
int main()
{
    cout<<"Vvedite kol-vo elementov"<<endl;
    int N;
    cin>>N;
    double n, sr_ar, sum;
    sum = 0;
    for (int i=0; i<N; ++i) {
        cin>>n;
        sum = sum + n;
    }
    sr_ar = sum/N;
    cout<<sr_ar<<endl;
    return 0;
}
