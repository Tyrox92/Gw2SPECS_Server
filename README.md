# Gw2SPECS_Server
Gw2SPECS Server V1.1 made by Tyrox and Kozzy is the server component for the [Gw2SPECS Client](https://github.com/Tyrox92/Gw2SPECS).

## Setup
- Port-forward the TCP port you want to use for this Program to the server you palan on running Gw2SPECS Server
- Start the server and type in your chosen port and confirm with enter (default port is 1234)
- Do not close the window while you want to use the server

Just start the Server and wait for the connections. ;)

## Docker Setup
- install [Docker](https://docs.docker.com/engine/installation/).
- Go to docker repository
- Build the docker image: ```docker build -t gw2spec-server``` .
- Run the server: ```docker run -d -p 1234:1234 gw2spec-server``` 

the server will be launch on the port 1234 by default if you want to change this port you can add the -e PORT=new-port-there and change the port number.
