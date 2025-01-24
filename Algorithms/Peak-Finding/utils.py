import os

def getOpenFilename(default = None):
    """
    Prompts the user to pick a file name. If the user doesn't enter a file name,
    returns the default.
    """

    prompt = "Enter a file name to load from"
    if default is not None:
        prompt += f" (default: {default})"
    prompt += ": "

    filename = input(prompt)
    if filename == "" and not (default is None):
        filename = default

    return filename

def getSaveFilename(default = None):
    """
    Prompts the user to pick a file name. If the user doesn't enter a file name,
    returns the default. If the file already exists, checks to make sure that
    the user wants to overwrite it.
    """

    prompt = "Enter a file name to save to"
    if default is not None:
        prompt += f" (default: {default})"
    prompt += ": "

    filename = input(prompt)
    if filename == "" and not (default is None):
        filename = default
    
    if os.path.exists(filename):
        print(f"The file {filename} already exists")
        prompt = "Overwrite (o), enter another name (f), or cancel (c)?"

        check = input(prompt)
        while (check != "o" and check != "f" and check != "c"):
            check = input(prompt)
        
        if check == "o":
            return filename
        elif check == "f":
            return getSaveFilename(default)
        elif check == "c":
            return None
        
    return filename