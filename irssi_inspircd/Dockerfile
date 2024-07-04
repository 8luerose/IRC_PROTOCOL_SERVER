# FROM ubuntu:20.04
FROM debian:bullseye

# Set environment variable to prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Change to Kakao mirror server for faster download speeds
RUN sed -i 's|http://archive.ubuntu.com/ubuntu/|http://mirror.kakao.com/ubuntu/|g' /etc/apt/sources.list  # 추가된 부분: Kakao 미러 서버 설정

# Update package list and install necessary packages
RUN apt-get update && \
    apt-get install -y sudo tzdata && \
	sudo apt-get install -y tcpflow && \
	sudo apt-get install -y netcat-openbsd && \
	sudo apt-get install -y vim

RUN echo "taehkwon: 'sudo', 'tzdata' Done" && \
	echo "taehkwon: Waiting.. install (1)inspircd and (2)irssi"

# Install inspircd and irssi
RUN sudo apt-get install -y inspircd && \
    echo "Asia/Seoul" > /etc/timezone && \
    ln -fs /usr/share/zoneinfo/Asia/Seoul /etc/localtime && \
    dpkg-reconfigure --frontend noninteractive tzdata && \
    sudo apt-get install -y irssi

# Modify the inspircd.conf to change the timeout and pingfreq
RUN sed -i 's/timeout="60"/timeout="86400"/g' /etc/inspircd/inspircd.conf && \
    sed -i 's/pingfreq="120"/pingfreq="86400"/g' /etc/inspircd/inspircd.conf

# Create required directory
RUN sudo mkdir -p /var/run/inspircd

# Copy the entrypoint script to the container
COPY start.sh /usr/local/bin/

# Make the entrypoint script executable
RUN chmod +x /usr/local/bin/start.sh

# Set the entrypoint
ENTRYPOINT ["/usr/local/bin/start.sh"]
