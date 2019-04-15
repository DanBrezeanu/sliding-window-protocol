## Sliding window network protocol

### Summary
An emulation of a sliding window protocol over a link between a sender and a reciever.

The network can:
- lose packages (the package's acknowledge timeouts)
- corrupt packages
- reorder packages

### Performances

For a 1MB file:
  - 20Mbs speed    10% Loss     0%  Reorder     10% Corrupt
  
     >1.63s
     
  - 10Mbs speed    10% Loss     10% Reorder     10% Corrupt
     >1.12s
  
