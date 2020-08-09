project(KernelArch C ASM)

aux_source_directory(${KernelArch_SOURCE_DIR}/${SynestiaArch} KernelArchSrc)
aux_source_directory(${KernelArch_SOURCE_DIR}/${SynestiaArch}/vmm/src KernelArchVMMSrc)

add_library(${PROJECT_NAME}
        OBJECT
        ${KernelArchSrc}
        ${KernelArchVMMSrc}
        ${KernelArch_SOURCE_DIR}/${SynestiaArch}/boot.S)

target_include_vmm_header_files(${PROJECT_NAME})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libc_header_files(${PROJECT_NAME})

if (SynestiaPlatform STREQUAL pi2)
	set(boardObj BoardRaspberryPi)
elseif (SynestiaPlatform STREQUAL pi3)
	set(boardObj BoardRaspberryPi)
elseif (SynestiaPlatform STREQUAL pi4)
	set(boardObj BoardRaspberryPi)
else ()
	message(ERROR "unexpected PLATFORM ${PLATFORM}")
endif ()

add_executable(${KernelName}
        $<TARGET_OBJECTS:KernelArch>
        $<TARGET_OBJECTS:Kernel>
        $<TARGET_OBJECTS:LibC>
        $<TARGET_OBJECTS:LibMath>
        $<TARGET_OBJECTS:LibGfx>
        $<TARGET_OBJECTS:LibGUI>
        $<TARGET_OBJECTS:Console>
        $<TARGET_OBJECTS:${boardObj}>)

target_link_options(${KernelName} PRIVATE -T ${KernelArch_SOURCE_DIR}/${SynestiaArch}/kernel.ld)

target_link_libraries(${KernelName} PRIVATE gcc)

add_custom_target(kernel.img ALL
	DEPENDS ${KernelName}
        COMMAND ${CMAKE_COMMAND} -E echo making kernel.img, wait a moment
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_OBJCOPY} ${KernelName} -O binary kernel.img)
