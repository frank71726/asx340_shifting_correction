#ifndef _i2c_H
#define _i2c_H

void IdleI2C( void ); 				
void StartI2C( void );
INT8S WriteI2C( INT8U data_out );
unsigned char ReadI2C( void );
void AckI2C( void );
void NotAckI2C( void );
void RestartI2C( void );
void StopI2C( void );
void I2C_Done(void);
void OpenI2C( INT8U, INT8U );
//void Initialize_I2C_Master(void);

#endif
