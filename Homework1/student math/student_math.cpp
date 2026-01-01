#include "student_math.h"


namespace
{
    constexpr double pi = 3.141592653589793;
    constexpr double half = 1.570796326794896;
    constexpr double two = 6.283185307179586;

    constexpr double eps = 1e-9;
    constexpr double INF = 1e10;

    double abs(double x) {return (x < 0.0) ? -x : x ;}
    double rad_to_deg(double rad) { return rad * 180.0 / pi; }
    double deg_to_rad(double deg) { return deg * pi / 180.0; }


    double reduce_in_pi(double x) // let x belongs to [-pi,pi]
    {
        while(x > pi) x -= two;
        while(x < -pi) x += two;
        return x;
    }

    double reduce_in_half(double x,int &flag) //flag = 1/0 ->cos/sin 
    {
        flag = 1; // sin(pi - x) = sin(x),cos(pi - x) = -cos(x)
        if(x > half)
        {
            x = pi - x;
            flag = -1;
        }
        else if(x < -half) //sin(-pi - x) = sin(x),cos(-pi - x) = -cos(x)
        {
            x = -pi - x;
            flag = -1;
        }
        //only cos need to reverse flag
        return x;
    }

    double Taylor_sin(double x) // ensure x in [-pi/2,pi/2]
    {
        double now = x; //term in series;
        double square = x * x;
        double sum = 0;

        sum += x;
        for(int i = 1;i <= 10;++i) //begin with second term
        {
            double div = (2.0 * i + 1.0) * (2.0 * i);
            now *= ((-square) / div); //i-th term
            sum += now;

            if(abs(now) < eps) break ;
        }
        return sum;
    }

    double Taylor_cos(double x) // ensure x in [-pi/2,pi/2]
    {
        double square = x * x,now = 1.0;
        double sum = 0;

        sum += now;
        for(int i = 1;i <= 10;++i)
        {
            double div = (2.0 * i - 1.0) * (2.0 * i);
            now *= ((-square) / div);
            sum += now;

            if(abs(now) < eps) break ;
        }
        return sum;
    }
}
namespace student_std
{
    double sin(double x)
    {
        double y = reduce_in_pi(x);
        int flag = 1;
        double val = reduce_in_half(y,flag);

        double ans = Taylor_sin(val);
        if(abs(ans) < eps) return 0.0;
        return ans;
    }

    double cos(double x)
    {
        double y = reduce_in_pi(x);
        int flag = 1;
        double val = reduce_in_half(y,flag);
        
        double ans = Taylor_cos(val);
        ans = 1.0 * flag * ans;
        if(abs(ans) < eps) return 0.0;
        return ans;
    }

    double tan(double x)
    {
        double s = sin(x),c = cos(x);
        double as = abs(s),ac = abs(c);

        if(ac <= as / (2.0 * INF)) // we get infinite value
        {
            int fs = -1,fc = -1;
            if(s >= 0.0) fs = 1;
            if(c >= 0.0) fc = 1;

            if(fs * fc > 0) return INF;
            return -INF;
        }

        double ans = s / c;
        if(abs(ans) < eps) return 0.0;
        return ans;
    }

    double cot(double x)
    {
        double s = sin(x),c = cos(x);
        double as = abs(s),ac = abs(c);

        if(as <= ac / (2.0 * INF)) // we get infinite value
        {
            int fs = -1,fc = -1;
            if(s >= 0.0) fs = 1;
            if(c >= 0.0) fc = 1;

            if(fs * fc > 0) return INF;
            return -INF;
        }

        double ans = c / s;
        if(abs(ans) < eps) return 0.0;
        return ans;
    }

    double sin_deg(double x) { return sin(deg_to_rad(x)); }
    double cos_deg(double x) { return cos(deg_to_rad(x)); }
    double tan_deg(double x) { return tan(deg_to_rad(x)); }
    double cot_deg(double x) { return cot(deg_to_rad(x)); }
}