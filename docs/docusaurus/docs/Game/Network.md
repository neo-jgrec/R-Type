---
sidebar_position: 4
---

# Network request
Packet description for each request.

- ### PlayerConnect
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Player id"
```
  - Request type = 0

  - Payload size = 1

- ### PlayerDisconnect
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Player id"
```
  - Request type = 1

  - Payload size = 1

- ### GameStart
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
```
  - Request type = 2

  - Payload size = 0

- ### GameOver
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
```
  - Request type = 3

  - Payload size = 0

- ### MapScroll
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-19: "Total scroll"
```
  - Request type = 4

  - Payload size = 4

- ### TileDestroy
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-19: "X"
20-23: "Y"
```
  - Request type = 5

  - Payload size = 8

- ### PlayerProjectileShoot
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "User id"
```
  - Request type = 6

  - Payload size = 1

- ### PlayerProjectileCreate
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Projectile id"
17-20: "X"
21-24: "Y"
```
  - Request type = 6

  - Payload size = 9


- ### PlayerProjectileDestroy
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Projectile id"
```
  - Request type = 6

  - Payload size = 1

- ### PlayerMissileShoot
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "User id"
```
  - Request type = 7

  - Payload size = 1

- ### PlayerMissileCreate
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Missile id"
17-20: "X"
21-24: "Y"
```
  - Request type = 7

  - Payload size = 9

- ### PlayerMissileDestroy
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Missile id"
```
  - Request type = 7

  - Payload size = 1

- ### PlayerMove
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Player id"
17-20: "X"
21-24: "Y"
```
  - Request type = 7

  - Payload size = 9

- ### PlayerCollide
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Player id"
17-20: "X"
21-24: "Y"
```
  - Request type = 8

  - Payload size = 9

- ### PlayerHit
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Player id"
```
  - Request type = 9

  - Payload size = 1

- ### PlayerDie
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Player id"
```
  - Request type = 10

  - Payload size = 1

- ### EnemySpawn
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Enemy id"
17-20: "X"
21-24: "Y"
```
  - Request type = 11

  - Payload size = 9

- ### EnemyMove
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Enemy id"
17-20: "X"
21-24: "Y"
```
  - Request type = 12

  - Payload size = 9

- ### EnemyDie
```mermaid
packet-beta
0-0: "Version"
1-1: "Request Type"
2-9: "Packet id"
10-11: "Payload size"
12-13: "Seq number"
14-15: "Total packet"
16-16: "Enemy id"
```
  - Request type = 13

  - Payload size = 1
