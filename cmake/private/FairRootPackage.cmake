################################################################################
# Copyright (C) 2021-2022 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  #
#                                                                              #
#              This software is distributed under the terms of the             #
#              GNU Lesser General Public Licence (LGPL) version 3,             #
#                  copied verbatim in the file "LICENSE"                       #
################################################################################

include(CMakePackageConfigHelpers)

# Configure/Install CMake package
function(install_cmake_package)
  if(FairCMakeModules_VERSION VERSION_GREATER_EQUAL 1.0.0)
    # FairCMakeModules 1.0.0 needed for OPTIONAL_COMPONENTS exporting
    fair_generate_package_dependencies()
  else()
    set(PACKAGE_DEPENDENCIES "# Not supported")
  endif()

  set(PACKAGE_INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION})
  if(PROJECT_EXPORT_SET)
    install(EXPORT ${PROJECT_EXPORT_SET}
      NAMESPACE ${PROJECT_NAME}::
      DESTINATION ${PACKAGE_INSTALL_DESTINATION}
      EXPORT_LINK_INTERFACE_LIBRARIES)
  endif()
  configure_package_config_file(
    "cmake/private/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${PACKAGE_INSTALL_DESTINATION}"
    PATH_VARS
      CMAKE_INSTALL_PREFIX
      CMAKE_INSTALL_FULL_BINDIR
      PACKAGE_INSTALL_DESTINATION)
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY AnyNewerVersion)
  install(
    FILES
      "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${PACKAGE_INSTALL_DESTINATION}")
endfunction()
