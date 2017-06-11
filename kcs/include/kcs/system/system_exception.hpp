//
// Created by dragonfly on 03/06/17.
//

#ifndef KCS_SYSTEM_SYSTEM_EXCEPTION_HPP
#define KCS_SYSTEM_SYSTEM_EXCEPTION_HPP

#include "../exception/exception_base.hpp"


namespace kcs {
namespace system {


/**
 * @brief       argparse_exception is the class that the arguement parser will use to throw
 *              exceptions.
 */
class system_exception : public kcs::exception::exception_base
{
public:
    /**
     * @brief       Constructor with parameters.
     * @param       str : Message to return by the exception object.
     */
    inline system_exception(const char* const str = nullptr) noexcept
            : exception_base(str)
    {
    }
};


}
}

#endif
