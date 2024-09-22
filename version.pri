defineReplace(build_version){
    OLD_VERSION = $$cat($$PWD/version.txt, lines)
    OLD_VERSION_LINES = $$split(OLD_VERSION, .)
    MAJAR_NUMBER = $$member(OLD_VERSION_LINES,0)
    MINOR_NUMBER = $$member(OLD_VERSION_LINES,1)
    MICRO_NUMBER = $$member(OLD_VERSION_LINES,2)
    BUILD_NUMBER =  $$member(OLD_VERSION_LINES,3)

    isEmpty(MAJAR_NUMBER){
        MAJAR_NUMBER = 0
    }
    isEmpty(MINOR_NUMBER){
        MINOR_NUMBER = 0
    }
    isEmpty(MICRO_NUMBER){
        MICRO_NUMBER = 0
    }
    isEmpty(BUILD_NUMBER){
        BUILD_NUMBER = 0
    }

    lessThan($$MAJAR_NUMBER, 1) {
        MAJAR_NUMBER = 1
    }

    lessThan($$MINOR_NUMBER, 1) {
        MINOR_NUMBER = 0
    }

    lessThan($$MICRO_NUMBER, 1) {
        MICRO_NUMBER  = $$system(git rev-list HEAD --count)
    }


    lessThan($$BUILD_NUMBER, 1) {
        BUILD_NUMBER = $$num_add($$BUILD_NUMBER,1)
    }

    VERSION = $$MAJAR_NUMBER"."$$MINOR_NUMBER"."$$MICRO_NUMBER"."$$BUILD_NUMBER
    message($$OLD_VERSION => $$VERSION)
    write_file($$PWD/version.txt, VERSION)
    return($$VERSION)
}

#调用版本号自动升级函数
!build_pass {
  VERSION = $$build_version()
}
