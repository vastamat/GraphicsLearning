#pragma once

namespace GameEngine
{
  ///Calculates FPS and also limits FPS
  class FpsLimiter
  {
  public:
    FpsLimiter();

    // Initializes the FPS limiter. For now, this is
    // analogous to setMaxFPS
    void Init(float _maxFPS);

    // Sets the desired max FPS
    void SetMaxFPS(float _maxFPS);

    //set the startTicks to the current frame ticks
    void BeginFrame();

    // end() will return the current FPS as a float
    float End();

  private:
    // Calculates the current FPS
    void CalculateFPS();

    float m_maxFPS{ 0.0f };
    float m_fps{ 0.0f };
    float m_frameTime{ 0.0f };
    unsigned int m_startTicks{ 0 };
  };
}