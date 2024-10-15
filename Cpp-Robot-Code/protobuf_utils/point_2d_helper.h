#ifndef POINT_2D_HELPER_H
#define POINT_2D_HELPER_H

#include <point_2d.h>
#include <worldmodel.pb.h>

template <class T>
inline void toProtobuf(const htwk::point_2d& p, T* w)
{
    w->set_x(p.x);
    w->set_y(p.y);
}

inline void fromProtobuf(htwk::point_2d& p, const protobuf::worldmodel::Point2D& w)
{
    p.x = w.x();
    p.y = w.y();
}

#endif // POINT_2D_HELPER_H
