#!/bin/bash

echo "taehkwon: Starting the InspIRCd server...\n"

# Start inspircd in the background
sudo inspircd --runasroot --nofork &

# Wait for 20 seconds to ensure the process ID is generated
echo "taehkwon: Waiting for 20 seconds to ensure the process ID is generated...\n"
sleep 20
echo "\ntaehkwon: Done!\n"

echo "taehkwon: Starting tcpflow to capture IRC traffic on port 6667...\n"
sudo tcpflow -i lo port 4000 -c &

# irssi -c 127.0.0.1 -p 6667 -n taehkwon
# docker exec -it irc /bin/bash

# Keep the container running
tail -f /dev/null
