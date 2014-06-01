 
#!/bin/bash

sudo chown -R $USER /opt/radit
sudo groupadd radit
sudo gpasswd -a $USER  radit
sudo chgrp -R radit /opt/radit
sudo chmod -R g+w /opt/radit 
