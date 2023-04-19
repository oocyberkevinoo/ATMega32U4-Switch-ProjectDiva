extern void PDAC_PC_Used_Update();

#include "Joystick.h"
USB_JoystickReport_Input_t ReportData;
USB_ServiceModeReport_Input_t SM_ReportDataIN;
USB_ServiceModeReport_Output_t SM_ReportDataOUT;
// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(void) {
  // We need to disable watchdog if enabled by bootloader/fuses.
  MCUSR &= ~(1 << WDRF);
  wdt_disable();
  // We need to disable clock division before initializing the USB hardware.
  clock_prescale_set(clock_div_1);
  // We can then initialize our hardware and peripherals, including the USB stack.
  // The USB stack should be initialized last.
  USB_Init();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
  // We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
  // We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {
  bool ConfigSuccess = true;

  // We setup the HID report endpoints.
  ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
  ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
  // We can read ConfigSuccess to indicate a success or failure at this point.
}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
<<<<<<< Updated upstream
     static uint8_t value = 0;
=======
     static byte command = 0x00;
     static byte value1 = 0x00;
     static byte value2 = 0x00;
>>>>>>> Stashed changes
/* get feature */
if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
{
  if (USB_ControlRequest.bRequest == HID_REQ_GetReport)
  {
<<<<<<< Updated upstream
      uint8_t feature_data[2] = {USB_ControlRequest.wValue & 0xFF};
      uint8_t feature_data_size;
      switch (USB_ControlRequest.wValue & 0xFF) /* contains report id */
      {
        case 4:
          feature_data_size = 2;
          feature_data[1] = value;
          PDM_PC = true;
=======
      uint8_t feature_data[3] = {USB_ControlRequest.wValue & 0xFF};
      uint8_t feature_data_size = 4;

      feature_data[1] = 0;
      feature_data[2] = 0;
      feature_data[3] = 0;
      
      switch (USB_ControlRequest.wValue & 0xFF) /* contains report id */
      {
        case 4: // Get if the controller already used the Manager

          switch(command){
            case PDM_PC_Used:  // Checking if it's a first time using manager
            feature_data[1] = PDM_PC_Used;
            feature_data[2] = ReadEEPROM(8);
            break;
            
            case PDM_PC_GetConfig:  // Get one configuration
            feature_data[1] = command;
            feature_data[2] = ReadEEPROM(value1);
            feature_data[3] = value1;
            break;
            
            case PDM_PC_SetConfig:  // Set one configuration
            feature_data[1] = command;
            feature_data[3] = ReadEEPROM(value1);
            WriteEEPROM(value1, value2);
            feature_data[2] = ReadEEPROM(value1);
            break;
            
            case PDM_PC_Reload: // Reload configurations
            PDAC_PC_RELOAD();
            break;

            case PDM_PC_Calibrate:  // Recalibrate the slider
            PDAC_PC_CALIBRATE();
            break;
            
            default:
            break;
          }
        

          
          
          //PDAC_PC_Used_Update();
>>>>>>> Stashed changes
          break;
        default:
          break;
      }
        Endpoint_ClearSETUP();
        /* Write the report data to the control endpoint */
        Endpoint_Write_Control_Stream_LE(feature_data, feature_data_size);
        Endpoint_ClearOUT();
        return;
  }
}
/* set feature */
else if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
{
  if (USB_ControlRequest.bRequest == HID_REQ_SetReport)
  {
        Endpoint_ClearSETUP();

        /* Wait until the LED report has been sent by the host */
        while (!(Endpoint_IsOUTReceived()))
        {
          if (USB_DeviceState == DEVICE_STATE_Unattached)
            return;
        }
        /* Read in from the host */
        uint8_t recv_data[64];
        /* Process the report */
      switch (USB_ControlRequest.wValue & 0xFF) /* contains report id */
      {
        case 4:
<<<<<<< Updated upstream
          Endpoint_Read_Control_Stream_LE(recv_data, 2);
          value = recv_data[1];
=======
          Endpoint_Read_Control_Stream_LE(recv_data, 4);
          command = recv_data[1];
          value1 = recv_data[2];
          value2 = recv_data[3];
>>>>>>> Stashed changes
          break;
        default:
          break;
      }

          Endpoint_ClearOUT();
          Endpoint_ClearStatusStage();
          
        return;
  }
}


  
  /*
  // We can handle two control requests: a GetReport and a SetReport.
<<<<<<< Updated upstream

  switch (USB_ControlRequest.bRequest) {

=======
  switch (USB_ControlRequest.bRequest) {
>>>>>>> Stashed changes
    case HID_REQ_SetReport:
    if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)){
      uint16_t ReportSize = USB_ControlRequest.wLength;
      uint8_t  ReportID   = (USB_ControlRequest.wValue & 0xFF);
      uint8_t  ReportType = (USB_ControlRequest.wValue >> 8) - 1;
      uint8_t  ReportData[ReportSize];
      //PDM_PC = true;
      if(ReportID == 0x04){
        //PDM_PC = true;
      }
    }
    break;
    
    case HID_REQ_GetReport:
    if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
      {
        uint16_t ReportSize = 0x08;
        uint8_t  ReportID   = (USB_ControlRequest.wValue & 0xFF);
        uint8_t  ReportType = (USB_ControlRequest.wValue >> 8) - 1;
        uint8_t ReportData = 0x15;
        //uint8_t  ReportData[HIDInterfaceInfo->Config.PrevReportINBufferSize];
<<<<<<< Updated upstream

        Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);

        Endpoint_ClearSETUP();

        if (ReportID)
          Endpoint_Write_8(120);

=======
        Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);
        Endpoint_ClearSETUP();
        if (ReportID)
          Endpoint_Write_8(120);
>>>>>>> Stashed changes
        Endpoint_Write_Control_Stream_LE(ReportData, ReportSize);
        Endpoint_ClearOUT();
        //PDM_PC = true;
        if(ReportID == 0x04){
          //PDM_PC = true;
        }
      }
    break;
    
  }*/

}

void HID_Task(void) {
  /* Device must be connected and configured for the task to run */
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

    void* Address = &ReportData;
    uint16_t Size = sizeof(ReportData);

    // We'll start with the OUT endpoint.
    Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
    // We'll check to see if we received something on the OUT endpoint.
    if (Endpoint_IsOUTReceived())
    {
      // If we did, and the packet has data, we'll react to it.
      if (Endpoint_IsReadWriteAllowed())
      {
        // We'll create a place to store our data received from the host.
        USB_JoystickReport_Output_t JoystickOutputData;
        // We'll then take in that data, setting it up in our storage.
        Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL);
        // At this point, we can react to this data.
        // However, since we're not doing anything with this data, we abandon it.
      }
      // Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
      Endpoint_ClearOUT();
    }

  
  /* Select the Joystick Report Endpoint */
  Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);

  /* Check to see if the host is ready for another packet */
  if (Endpoint_IsINReady()) {
    /* Write Joystick Report Data */
    Endpoint_Write_Stream_LE(Address, Size, NULL);

    /* Finalize the stream transfer to send the last packet */
    Endpoint_ClearIN();
    /* Clear the report data afterwards */
    memset(Address, 0, Size);
  }
}

// Process and deliver data from IN and OUT endpoints.
void HID_Task_old(void) {
  // If the device isn't connected and properly configured, we can't do anything here.
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

  // We'll start with the OUT endpoint.
  Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
  // We'll check to see if we received something on the OUT endpoint.
  if (Endpoint_IsOUTReceived())
  {
    // If we did, and the packet has data, we'll react to it.
    if (Endpoint_IsReadWriteAllowed())
    {
      if(PDM_PC){
     Endpoint_Read_Stream_LE(&SM_ReportDataOUT, sizeof(SM_ReportDataOUT), NULL);
     
    }
    else{
     // We'll create a place to store our data received from the host.
      USB_JoystickReport_Output_t JoystickOutputData;
      // We'll then take in that data, setting it up in our storage.
      Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL);
      // At this point, we can react to this data.
      // However, since we're not doing anything with this data, we abandon it.

      // Key data to pass into SERVICE MODE
<<<<<<< Updated upstream
      if(JoystickOutputData.LX == 0x39 && JoystickOutputData.RX == 0x39 && JoystickOutputData.HAT == 0x39 && !PDM_PC)
        PDM_PC = true;
=======
      //if(JoystickOutputData.LX == 0x39 && JoystickOutputData.RX == 0x39 && JoystickOutputData.HAT == 0x39 && !PDM_PC)
        //PDM_PC = true;
>>>>>>> Stashed changes
    }
      
    }
    // Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
    Endpoint_ClearOUT();
  }

  // We'll then move on to the IN endpoint.
  Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);
  // We first check to see if the host is ready to accept data.
  if (Endpoint_IsINReady())
  {
    // Once populated, we can output this data to the host. We do this by first writing the data to the control stream.
    if(PDM_PC){
     Endpoint_Write_Stream_LE(&SM_ReportDataIN, sizeof(SM_ReportDataIN), NULL);
    }
    else{
     Endpoint_Write_Stream_LE(&ReportData, sizeof(ReportData), NULL);
    }

    
     // We then send an IN packet on this endpoint.
    Endpoint_ClearIN();
    /* Clear the report data afterwards */
    memset(&ReportData, 0, sizeof(ReportData));
  }
}
