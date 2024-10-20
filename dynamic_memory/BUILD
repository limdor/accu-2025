cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    # https://www.boost.org/doc/libs/master/doc/html/stacktrace/configuration_and_build.html
    defines = ["BOOST_STACKTRACE_USE_BACKTRACE"],
    linkopts = ["-lbacktrace"],
    deps = [
        "@boost.stacktrace"
    ]
)
