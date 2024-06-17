FROM debian:bullseye

# Set environment variable to prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install necessary packages
RUN apt-get update && \
    apt-get install -y sudo tzdata && \
    apt-get install -y tcpflow inspircd netcat && \
    echo "Asia/Seoul" > /etc/timezone && \
    ln -fs /usr/share/zoneinfo/Asia/Seoul /etc/localtime && \
    dpkg-reconfigure --frontend noninteractive tzdata

# Create required directories
RUN sudo mkdir -p /var/run/inspircd /etc/inspircd

# Create the inspircd.conf file with the desired content
# RUN echo '<connect>\n    pingfreq="60"\n    timeout="180"\n</connect>' > /etc/inspircd/inspircd.conf

# Copy the entrypoint script to the container
COPY start.sh /usr/local/bin/

# Make the entrypoint script executable
RUN chmod +x /usr/local/bin/start.sh

# Set the entrypoint
ENTRYPOINT ["/usr/local/bin/start.sh"]


# FROM debian:bullseye

# # Set environment variable to prevent interactive prompts
# ENV DEBIAN_FRONTEND=noninteractive

# # Update package list and install necessary packages
# RUN apt-get update && \
#     apt-get install -y sudo tzdata && \
#     apt-get install -y tcpflow inspircd netcat && \
#     echo "Asia/Seoul" > /etc/timezone && \
#     ln -fs /usr/share/zoneinfo/Asia/Seoul /etc/localtime && \
#     dpkg-reconfigure --frontend noninteractive tzdata

# # Create required directories
# RUN sudo mkdir -p /var/run/inspircd /etc/inspircd

# # Create the inspircd.conf file with the desired content
# RUN echo '<bind address="*" port="6667" type="clients" />\n<connect allow="*" pingfreq="60" timeout="180" />' > /etc/inspircd/inspircd.conf

# # Copy the entrypoint script to the container
# COPY start.sh /usr/local/bin/

# # Make the entrypoint script executable
# RUN chmod +x /usr/local/bin/start.sh

# # Set the entrypoint
# ENTRYPOINT ["/usr/local/bin/start.sh"]
