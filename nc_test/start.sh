#!/bin/bash

echo "taehkwon: Starting the InspIRCd server..."

# Start inspircd in the background
sudo inspircd --runasroot --nofork &

# Wait for 20 seconds to ensure the process ID is generated
echo "taehkwon: Waiting for 20 seconds to ensure the process ID is generated..."
sleep 20
echo "taehkwon: Done!"

echo "taehkwon: Starting tcpflow to capture IRC traffic on port 6667..."
sudo tcpflow -i lo port 6667 -c &

# Keep the container running
tail -f /dev/null
