#ifndef RESOURCE_H
#define RESOURCE_H
#include <QString>

enum class Resource
{
    Wood, Stone, Metal
};

namespace Reward {
    Resource get_resource(const QString& name);
    QString to_qstring(const Resource& r);
}

std::ostream& operator << (std::ostream& out, const Resource& resource);

#endif // RESOURCE_H
