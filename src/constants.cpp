#include "constants.h"



unsigned int shaderProgram = 0;

void setMat4(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}



float myRound(float var, int rounding_point) {
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67

    int rounding_number = 1;

    rounding_number = pow(10, rounding_point);
    
    float value = (int)(var * rounding_number + .5);
    return (float)value / rounding_number;
}