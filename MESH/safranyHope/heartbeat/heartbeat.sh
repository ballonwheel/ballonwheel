#!/usr/bin/env bash
# Használat: ./safrany_rsync.sh <relay-ip>
# Példa: ./safrany_rsync.sh 144.256.333.444

# crontab -e
# 0 5 * * * /home/imre/ballonwheel/MESH/safranyHope/heartbeat/safrany_rsync.sh ipadd >>/var/log/safranyHeartbeat/safrany_rsync.log 2>&1


set -euo pipefail

if [ $# -ne 1 ]; then
    echo "Használat: $0 <relay-ip>"
    exit 1
fi

RELAY_IP="$1"
SSH_KEY="$HOME/.ssh/id_ed25519_rpiz2w3_250829"
SSH_PORT=2202
SSH_USER="imre"

RSYNC_OPTS="-a -z --info=progress2"

rsync $RSYNC_OPTS \
    -e "ssh -p ${SSH_PORT} -o ProxyCommand='ssh -i ${SSH_KEY} -o IdentitiesOnly=yes -W %h:%p relay@${RELAY_IP}'" \
    ${SSH_USER}@127.0.0.1:/var/log/safranyTemp/ \
    ${SSH_USER}@127.0.0.1:/var/log/safranyGpio/ \
    ${SSH_USER}@127.0.0.1:/var/log/soil/ \
    .
