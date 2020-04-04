cmake_minimum_required(VERSION 3.10)

project(MakeVirtualFileSystem VERSION 0.1.0 LANGUAGES CSharp)

set(MAKEVFS_SOURCES
	"App.config"
	"Extensions/ByteArrayExtensions.cs"
	"Extensions/StringExtensions.cs"
	"Extensions/SystemTypesExtensions.cs"
	"VFS/Extensions/VirtualEntriesFilePartEntryExtensions.cs"
	"VFS/Extensions/VpkFileEntryExtensions.cs"
	"Program.cs"
	"Properties/AssemblyInfo.cs"
	"Utility/CommandLine.cs"
	"Utility/ICommandLine.cs"
	"Utility/ILoggingFacility.cs"
	"Utility/ILoggingFacilityExecutor.cs"
	"Utility/LoggingFacility.cs"
	"VFS/IFileProcess.cs"
	"VFS/Internal/TableHeaderEntry.cs"
	"VFS/Internal/VpkFileEntry.cs"
	"VFS/ListFileProcess.cs"
	"VFS/PackFileProcess.cs"
	"VFS/UnpackFileProcess.cs"
	"VFS/Internal/VirtualEntriesFilePartEntry.cs")

add_executable(${PROJECT_NAME} ${MAKEVFS_SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY LANGUAGE 7.3)
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2")
set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE TRUE)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DOTNET_REFERENCES
    "System"
    "Microsoft.CSharp"
    "System.Xml")

## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})