#!/usr/bin/python
# Import library files
import argparse
import json
import time
import requests
import requests.packages.urllib3


def login(server, userid, domain, password):
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
    picture = "https://{0}/api/v1/session/network-login"
    url = picture.format(server)

    # Use a Python dictionary to send all the fields of the request body
    # for HTTP request.
    login = dict()
    login["user_id"] = userid
    login["domain"] = domain
    login["password"] = password
    login["persona"] = 'user'

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
    user_obj = dictionary["user"]

    # Return it for later use.
    return x_auth_token, user_obj["preferred_database"]


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
    parser.add_argument("domain", help="the domain to use to log in")
    parser.add_argument("password", help="the password to use to log in")

    # Parameters for target workspace and folder
    parser.add_argument("count", help="loop for <count> times")

    # Parse the arguments according to the given configuration.
    return parser.parse_args()


def reserve_doc_num(db, token):
    picture = "https://{0}/api/v1/documents/{1}/reserve"
    url = picture.format(args.server, db)

    headers = dict()

    headers["X-Auth-Token"] = x_auth_token
    response = requests.get(url, headers=headers, verify=False)

    dictionary = response.json()
    return dictionary["data"]["document_number"]


if __name__ == '__main__':
    args = parse_arguments()

    # Using self-signed certificate - see: https://github.com/shazow/urllib3/issues/497#issuecomment-61354544
    # urllib3.disable_warnings()
    requests.packages.urllib3.disable_warnings()

    #x_auth_token, database = login(args.server, args.userid, args.domain, args.password)
    x_auth_token = 'wPOo4B+87VkhIJA07ygAzJVdTmZ4ezKDvcoyTJHw6fZkQ0w4Qj31HZQ/Iec615F1'
    database = 'Active'


    t0 = time.clock()
    for n in range(0, int(args.count)):
        print reserve_doc_num(database, x_auth_token)
    t1 = time.clock()

    print "Incremented ", args.count, " docnums in ", t1-t0, " seconds!"
