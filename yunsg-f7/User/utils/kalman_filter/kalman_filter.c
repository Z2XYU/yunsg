 #include "kalman_filter.h"

 void kalman_init(KalmanFilter_t *kf,float q,float r,float initial_value)
 {
    kf->q=q;
    kf->r=r;
    kf->x=initial_value;
    kf->p=1.0f;
 }

 float kalman_update(KalmanFilter_t *kf,float measurement)
 {
    //预测
    kf->p= kf->p+kf->q;

    //计算卡尔曼增益
    kf->k= kf->p / (kf->p + kf->r);

    //更新估计值
    kf->x = kf->x+ kf->k *(measurement - kf->x);

    //更新误差协方差
    kf->p = (1-kf->k)*kf->p;

    return kf->x;
 }