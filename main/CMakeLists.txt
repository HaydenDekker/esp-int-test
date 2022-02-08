idf_component_register(SRCS "hello_world_main.c" "highint5.S"
                    INCLUDE_DIRS "")
                    
target_link_libraries(${COMPONENT_TARGET} "-u ld_include_xt_highint5")