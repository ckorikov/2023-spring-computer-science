#include "config.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace config
{
    namespace pt = boost::property_tree;

    std::string get_token(std::string filename)
    {
        pt::ptree root;
        pt::read_json(filename, root);
        std::string token = root.get<std::string>("token");
        return token;
    }

}
