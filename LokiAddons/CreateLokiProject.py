# Creates a new Loki project from template files

import argparse
import os
import shutil
import fileinput
import uuid

def createLokiModuleProject(project_name, project_description, project_major_version, project_minor_version, project_patch_version):
    base_project_dir = os.path.abspath(os.path.join(os.getcwd(), project_name))
    includes_dir = os.path.join(base_project_dir, "inc")
    source_dir = os.path.join(base_project_dir, "src")

    # copy template directory to its new home
    print("Creating: {0}".format(base_project_dir))
    shutil.copytree(".\LokiModuleTemplate", base_project_dir)

    # rename template files to match new project name
    print("Renaming Visual Studio project template files")
    os.rename(os.path.join(base_project_dir, "LokiModuleTemplate.vcxproj"), os.path.join(base_project_dir, project_name + ".vcxproj"))
    os.rename(os.path.join(base_project_dir, "LokiModuleTemplate.vcxproj.filters"), os.path.join(base_project_dir, project_name + ".vcxproj.filters"))

    project_guid = str(uuid.uuid1())
    print("Using project GUID: {0}".format(project_guid))

    # replace tokens in the project file
    print("Adding project name and GUID to: {0}".format(os.path.join(base_project_dir, project_name + ".vcxproj")))
    for line in fileinput.input(os.path.join(base_project_dir, project_name + ".vcxproj"), inplace=1):
        line = line.replace("$ProjectGuid$", project_guid)
        line = line.replace("$LokiModuleName$", project_name)
        print(line),

    # replace tokens in the .filters file
    print("Adding new GUIDs and project name to: {0}".format(os.path.join(base_project_dir, project_name + ".vcxproj.filters")))
    for line in fileinput.input(os.path.join(base_project_dir, project_name + ".vcxproj.filters"), inplace=1):
        line = line.replace("$SourceGuid$", str(uuid.uuid1()))
        line = line.replace("$HeaderGuid$", str(uuid.uuid1()))
        line = line.replace("$ResourceGuid$", str(uuid.uuid1()))
        line = line.replace("$LokiModuleName$", project_name)
        print(line),

    # rename source and header files
    print("Renaming C++ template files")
    os.rename(os.path.join(includes_dir, "LokiModuleTemplate.hpp"), os.path.join(includes_dir, project_name + ".hpp"))
    os.rename(os.path.join(source_dir, "LokiModuleTemplate.cpp"), os.path.join(source_dir, project_name + ".cpp"))

    # replace tokens in the hpp file
    print("Updating header file...")
    for line in fileinput.input(os.path.join(includes_dir, project_name + ".hpp"), inplace=1):
        line = line.replace("$LokiModuleName$", project_name)
        print(line),

    # replace tokens in the cpp file
    print("Updating source file...")
    for line in fileinput.input(os.path.join(source_dir, project_name + ".cpp"), inplace=1):
        line = line.replace("$LokiModuleName$", project_name)
        line = line.replace("$LokiModuleDescription$", project_description)
        line = line.replace("$MajorVersionNumber$", str(project_major_version))
        line = line.replace("$MinorVersionNumber$", str(project_minor_version))
        line = line.replace("$PatchVersionNumber$", str(project_patch_version))
        print(line),
    
def main():
    parser = argparse.ArgumentParser(description='Creates a new Loki module project from a template.')
    parser.add_argument("project_name", help="The name of the Loki project to add. Required.")
    parser.add_argument("-d", "--description", help="A description of the project. Default is an empty string.", type=str, default="")
    parser.add_argument("-vmaj", "--major_version", help="The major revision number of the project. Default is 0.", type=int, default=0)
    parser.add_argument("-vmin", "--minor_version", help="The minor revision number of the project. Default is 0.", type=int, default=0)
    parser.add_argument("-vpat", "--patch_version", help="The patch revision number of the project. Default is 0.", type=int, default=0)
    args = parser.parse_args ()
    createLokiModuleProject(args.project_name, args.description, args.major_version, args.minor_version, args.patch_version)

if __name__ == "__main__":
    main ()

