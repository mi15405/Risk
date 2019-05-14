#include <iostream>
#include "resource.h"

Resource Reward::get_resource(const QString& resource)
{
    if (resource == "drvo")
        return Resource::Wood;
    else if (resource == "kamen")
        return Resource::Stone;
    else if (resource == "metal")
        return Resource::Metal;
    else
    {
        std::cout << "Ne postoji resurs" << resource.toStdString() << std::endl;
        return Resource::Wood;
    }
}

QString Reward::to_qstring(const Resource& r)
{
    if (r == Resource::Wood)
        return "WOOD";
    else if (r == Resource::Stone)
        return "STONE";
    else if (r == Resource::Metal)
        return "METAL";
    else
        return "UNKNOWN";
}

std::ostream& operator << (std::ostream& out, const Resource& resource)
{
    switch (resource) {
    case Resource::Wood:
        out << "WOOD";
        break;

    case Resource::Stone:
        out << "STONE";
        break;

    case Resource::Metal:
        out << "METAL";
        break;

    default:
        out << "UNKNOWN RESOURCE";
        break;
    }

    return out;
}
