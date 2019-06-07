# Custom script to search for matching workspaces, create legacy folders and import documents from a network share
# Import library files 
import argparse 
import json 
import datetime
import requests
import requests.packages.urllib3

    
def parse_arguments():
    ''' 
    The following function parses the command-line arguments input to the script. 
    The argparse.ArgumentParser function halts the program and prints usage details if invalid 
    parameters are input to the script. 
    :return: an argparse.Namespace containing the command-line arguments 
    '''
    # This parser is very useful in processing command-line arguments to 
    # Python scripts, and is now a part of the standard Python libraries. 
    parser = argparse.ArgumentParser()
 
    # Specify the command-line arguments that can be used with this script. 
    parser.add_argument("server", help="the server name of the Work server") 
    parser.add_argument("userid", help="the user ID to use to log in") 
    parser.add_argument("password", help="the password to use to log in")
    parser.add_argument("container_id", help="the workspace or template ID to create subfolders under")
    parser.add_argument("start", help="the offset to start for appending to the folder name")
    parser.add_argument("count", help="the number of folders to create")
 
    # Parse the arguments according to the given configuration. 
    return parser.parse_args()

    
def login(server, userid, password):
    ''' 
    The following function logs in to the Work REST API. 
    :input parameters 
          server: login server name 
          userid: valid user ID 
          password: password for the user ID 
    :return: Authorization token, to be used in subsequent calls in the 
             X-Auth-Token HTTP header. 
    '''
    # Create "format pictures" that describe the form of REST API calls, 
    # and fill in the variables. 
    picture = "https://{0}/api/v1/session/login" 
    url = picture.format(server)
 
    # Use a Python dictionary to send all the fields of the request body 
    # for HTTP request. 
    login = dict() 
    login["user_id"] = userid 
    login["password"] = password
 
    # Convert the dictionary into JSON to send it only in one call. 
    data_to_post = json.dumps(login).encode('UTF-8')

    # Likewise, use a Python dictionary to send headers for the HTTP request. 
    headers = dict()
 
    # The information sent is in JSON. Inform the server through the 
    # "Content-Type" header. 
    headers["Content-Type"] = "application/json"
 
    # The URL, the request body, and the headers make up a request. 
    response = requests.put(url, data_to_post, headers=headers, verify=False)

    # Send the request and get the response in JSON dictionary. 
    dictionary = response.json()

    # Retrieve the authorization token from the response dictionary. 
    x_auth_token = dictionary["X-Auth-Token"]
 
    # Return it for later use. 
    return x_auth_token

    
def create_folder(server, token, container, name):
    picture = "https://{0}/api/v2/customers/1/libraries/{1}/folders/{2}/subfolders"
    database = container.split('!')[0]
    url = picture.format(server, database, container)
 
    headers = {"X-Auth-Token": token, "Content-Type": "application/json"}
    
    folder_profile = dict()
    folder_profile["name"] = name
    folder_profile["description"] = "Created using Postman on " + str(datetime.datetime.now())
    folder_profile["database"] = database
    folder_profile["default_security"] = "inherit"

    data_to_post = json.dumps(folder_profile).encode('UTF-8')

    response = requests.post(url, data_to_post, headers=headers, verify=False)
    json_response = response.json()
 
    folder_result = json_response['data']
    print folder_result
    

# The following is the main program which invokes all other functions. 
if __name__ == '__main__':
    # Parse the command-line arguments. 
    args = parse_arguments()

    # Using self-signed certificate - see: https://github.com/shazow/urllib3/issues/497#issuecomment-61354544    
    # urllib3.disable_warnings()
    requests.packages.urllib3.disable_warnings()

    # Log onto DMS and obtain token. 
    x_auth_token = login(args.server, args.userid, args.password)
     
    # loop through each matter in the list
    for c in range(int(args.start), int(args.start)+int(args.count)):
        create_folder(args.server, x_auth_token, args.container_id, "Folder #" + str(c))
        
