//////////////////////////////////////
//                                  //
//   rootlogon for NFADC400 Data    //
//                                  //
//  Author: Genie Jhang             //
// Contact: geniejhang@majimak.com  //
//    Date: 2013. 11. 06            //
//                                  //
//////////////////////////////////////

void rootlogon()
{
  cout << endl;
  cout << " //////////////////////////////////////////////////" << endl;
  cout << " //                                              //" << endl;
  cout << " // Plotting script is loaded!                   //" << endl;
  cout << " //                                              //" << endl;
  cout << " //   Note)                                      //" << endl;
  cout << " //     If you want to read the data in other    //" << endl;
  cout << " //     folder, you should copy the share        //" << endl;
  cout << " //     library file, libNFADC400.so.            //" << endl;
  cout << " //                                              //" << endl;
  cout << " //////////////////////////////////////////////////" << endl;
  cout << endl;

  cout << endl;
  cout << " //////////////////////////////////////////////////" << endl;
  cout << " //                                              //" << endl;
  cout << " //   Function List)                             //" << endl;
  cout << " //     - load(\"DATAFILE.root\");                 //" << endl;
  cout << " //     - plot(moduleNum, channelNum, eventNum)  //" << endl;
  cout << " //     - convert()                              //" << endl;
  cout << " //                                              //" << endl;
  cout << " //   Note)                                      //" << endl;
  cout << " //     To see the channel setting information,  //" << endl;
  cout << " //     for example, that of channel 1 of mod-   //" << endl;
  cout << " //     ule 2, type 'Mod2 -> Print(1)' after     //" << endl;
  cout << " //     load().                                  //" << endl;
  cout << " //                                              //" << endl;
  cout << " //////////////////////////////////////////////////" << endl;
  cout << endl;

  if (gSystem -> AccessPathName("libNFADC400.so")) {
    cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << " !!                                                 !!" << endl;
    cout << " !! Type 'make shared' to build the shared library! !!" << endl;
    cout << " !!                                                 !!" << endl;
    cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << endl;
  }
}
