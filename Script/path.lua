CurrentWorkingDirectory = os.getcwd()
RootPath = string.sub(CurrentWorkingDirectory, 0, string.len(CurrentWorkingDirectory) - string.len("Script") - 1)
EnginePath = path.join(RootPath, "Engine")

SourcePath = path.join(EnginePath, "Source")
BinaryPath = path.join(EnginePath, "Binary/bin")
IntermediatePath = path.join(EnginePath, "Binary/obj")

EditorPath = path.join(SourcePath, "Editor")
RuntimePath = path.join(SourcePath, "Runtime")
ThirdPartyPath = path.join(SourcePath, "ThirdParty")

print("Root path: "..RootPath)
print("Engine path: "..EnginePath)

print("Source path: "..SourcePath)
print("Binary path: "..BinaryPath)
print("Intermediate path: "..IntermediatePath)

print("Editor path: "..EditorPath)
print("Runtime path: "..RuntimePath)
print("Thirdparty path: "..ThirdPartyPath)

print("")
