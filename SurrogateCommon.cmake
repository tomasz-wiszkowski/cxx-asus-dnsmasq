# Fake libnvram.so - does not perform any actual operation, but serves as a
# target to link against.
# This is not perfect, and in fact we should use the actual shared object from
# the target device, but our requirements are also rather slim, so may as well
# simulate that we have one.
add_library(nvram SHARED
    src/mock/nvram.cc
)

include(GitRepoState)

git_get_sha(GIT_SHA)
git_get_state(GIT_STATE)

configure_file(src/version.h.in src/version.h)

# Core logic to be used with tests.
add_library(surrogate STATIC
    src/dnsmasq_config.cc
    src/host_info.cc
    src/process_nvram.cc
)

