// Copyright (c) 2019, QuantStack and Mamba Contributors
//
// Distributed under the terms of the BSD 3-Clause License.
//
// The full license is in the file LICENSE, distributed with this software.

#ifndef MAMBA_API_INSTALL_HPP
#define MAMBA_API_INSTALL_HPP

#include "mamba/core/context.hpp"
#include "mamba/core/mamba_fs.hpp"
#include "mamba/core/package_cache.hpp"
#include "mamba/core/package_info.hpp"
#include "mamba/core/pool.hpp"
#include "mamba/core/repo.hpp"
#include "mamba/core/solver.hpp"

#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

#include <string>
#include <vector>


namespace mamba
{
    void install();

    void install_specs(const std::vector<std::string>& specs,
                       bool create_env = false,
                       int solver_flag = SOLVER_INSTALL,
                       int is_retry = 0);

    void install_explicit_specs(const std::vector<std::string>& specs, bool create_env = false);
    void install_lockfile_specs(const fs::u8path& lockfile_specs, bool create_env = false);

    namespace detail
    {
        void create_target_directory(const fs::u8path prefix);

        void create_empty_target(const fs::u8path& prefix);

        void file_specs_hook(std::vector<std::string>& file_specs);

        void channels_hook(std::vector<std::string>& channels);

        bool download_explicit(const std::vector<PackageInfo>& pkgs, MultiPackageCache& pkg_caches);

        struct other_pkg_mgr_spec
        {
            std::string pkg_mgr;
            std::vector<std::string> deps;
            std::string cwd;
        };

        bool operator==(const other_pkg_mgr_spec& s1, const other_pkg_mgr_spec& s2);

        struct yaml_file_contents
        {
            std::string name;
            std::vector<std::string> dependencies, channels;
            std::vector<other_pkg_mgr_spec> others_pkg_mgrs_specs;
        };

        bool eval_selector(const std::string& selector);

        yaml_file_contents read_yaml_file(fs::u8path yaml_file);

        std::tuple<std::vector<PackageInfo>, std::vector<MatchSpec>> parse_urls_to_package_info(
            const std::vector<std::string>& urls);

        inline void to_json(nlohmann::json&, const other_pkg_mgr_spec&)
        {
        }
    }

}

namespace YAML
{
    template <>
    struct convert<mamba::detail::other_pkg_mgr_spec>
    {
        static Node encode(const mamba::detail::other_pkg_mgr_spec& /*rhs*/)
        {
            return Node();
        }

        static bool decode(const Node& /*node*/, mamba::detail::other_pkg_mgr_spec& /*rhs*/)
        {
            return true;
        }
    };
}

#endif
