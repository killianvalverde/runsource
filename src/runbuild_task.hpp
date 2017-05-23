//
// Created by Killian on 22/05/17.
//

#ifndef RUNSOURCE_RUNBUILD_TASK_HPP
#define RUNSOURCE_RUNBUILD_TASK_HPP

#include <experimental/filesystem>
#include <unordered_set>
#include <vector>

#include "kcs/argparse.hpp"
#include "language.hpp"

namespace stdfs = std::experimental::filesystem;
namespace kap = kcs::argparse;


namespace runsource {


class runbuild_task
{
public:
    runbuild_task(const kap::arg_parser& ap);
    bool is_c() const noexcept;
    bool is_cpp() const noexcept;
    int do_operation() const;

private:
    int build_c(std::string output_name, bool verbose) const;
    int exec_c() const;
    int build_cpp(std::string output_name, bool verbose) const;
    int exec_cpp() const;

private:
    std::vector<stdfs::path> files_;
    bool build_;
    language language_;
    std::string c_standard_;
    std::string cpp_standard_;
    std::vector<std::string> compiler_args_;
    std::vector<std::string> program_args_;
    
    static std::unordered_set<std::string> c_extensions_;
    static std::unordered_set<std::string> cpp_extensions_;
};


}


#endif
