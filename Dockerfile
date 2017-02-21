# Use the libyui/devel image as the base
FROM libyui/devel
RUN zypper --non-interactive install --no-recommends libmicrohttpd-devel jsoncpp-devel
COPY . /usr/src/app
