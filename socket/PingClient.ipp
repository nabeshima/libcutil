/**
 * $Id: PingClient.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cstring>

namespace cutil {

inline uint16_t PingClient::checksum(uint16_t *buf, int size) {
  uint64_t sum = 0;

  while (size > 1) {
    sum += *buf;
    ++buf;
    size -= 2;
  }

  if (size == 1) {
    sum += *(uint16_t *)buf;
  }

  sum = (sum & 0xffff) + (sum >> 16);
  sum = (sum & 0xffff) + (sum >> 16);

  return ~sum;
}

inline bool PingClient::ping(const Address &peer, uint64_t timeout_usec) {
  Socket socket;
  if (!socket.open(peer.getFamily(), RAW)) {
    return false;
  }

  socket.connect(peer);

  {
    icmp hdr;
    ::memset(&hdr, 0, sizeof(hdr));
    hdr.icmp_type = ICMP_ECHO;
    hdr.icmp_code = 0;
    hdr.icmp_cksum = 0;
    hdr.icmp_hun.ih_idseq.icd_id = 0;
    hdr.icmp_hun.ih_idseq.icd_seq = htons(10);
    hdr.icmp_cksum = checksum((uint16_t *)&hdr, sizeof(hdr));

    if (socket.send(&hdr, sizeof(hdr)) < 1) {
      return false;
    }
  }
  {
    char buf[2000];
    memset(buf, 0, sizeof(buf));

    if (!socket.isReceivable(timeout_usec) ||
        socket.recv(buf, sizeof(buf)) < 1) {
      return false;
    }

    ip *iphdrptr = (ip *)buf;
    icmp *icmphdrptr = (icmp *)(buf + (iphdrptr->ip_hl * 4));

    return (icmphdrptr->icmp_type == ICMP_ECHOREPLY);
  }
}
}
