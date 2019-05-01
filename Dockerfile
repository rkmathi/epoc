FROM ubuntu:18.04

# Configure working directory
ADD . /work
WORKDIR /work

# Install packages
RUN apt-get update && \
  apt-get upgrade -y && \
  apt-get install -y build-essential cmake gcc g++ gdb gdbserver openssh-server rsync

# Enable ssh for root user
# see also: https://docs.docker.com/engine/examples/running_ssh_service/
RUN mkdir /var/run/sshd && \
  echo 'root:root' | chpasswd && \
  sed -i 's/#\?PermitRootLogin prohibit-password/PermitRootLogin yes/'   /etc/ssh/sshd_config && \
  sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

# Expose port for ssh & gdbserver
EXPOSE 22 7777
# Expose port for server
EXPOSE 3000

# Run sshd
CMD ["/usr/sbin/sshd", "-D"]
