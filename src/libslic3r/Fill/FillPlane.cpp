

#include "../ClipperUtils.hpp"
#include "../ExPolygon.hpp"
#include "../ShortestPath.hpp"
#include "../Surface.hpp"

#include "FillPlane.hpp"

namespace Slic3r {

void FillPlane::_fill_surface_single(
    const FillParams                &params,
    unsigned int                     thickness_layers,
    const std::pair<float, Point>   &direction,
    ExPolygon                        expolygon,
    Polylines                       &polylines_out)
{
    float nozzle=0.0f;
    if(this->print_config->nozzle_diameter.is_vector())
    {
        const ConfigOptionFloats * nozzles=& (this->print_config->nozzle_diameter);
        nozzle=nozzles->get_at(0);
    }
    else
    {
        nozzle=this->print_config->nozzle_diameter.getFloat();
    }

    double linedst=scale_(nozzle/params.density);

    double asin=std::sin(this->angle);
    double acos=std::cos(this->angle);

    Vec2d direct(asin,acos);
    Vec2d prepend(-acos,asin);
    BoundingBox bounding_box = expolygon.contour.bounding_box();

    Vec2d center=bounding_box.center().cast<double>();
    Vec2d ground_center=center-prepend*scale_(this->z);
    double center_offset=ground_center.dot(prepend);
    center_offset=center_offset-std::round(center_offset/linedst)*linedst; //offset from ground line grid.

    center-=prepend*center_offset;

    double diagonal=2*bounding_box.size().maxCoeff(); //unable to compute norm, so use 2* max
    int lcount=std::floor(diagonal/linedst);

    Polylines polylines_src;
    for(int i=-lcount/2;i<=lcount/2;i++)
    {
        Vec2d off=center+prepend*i*linedst;

        polylines_src.push_back(Polyline());
        Points &pts = polylines_src.back().points;
        pts.reserve(2);
        pts.push_back((off-direct*diagonal).cast<int>());
        pts.push_back((off+direct*diagonal).cast<int>());
    }

    polylines_out = intersection_pl(polylines_src, offset(expolygon, scale_(0.5*nozzle)));    

}

} // namespace Slic3r










