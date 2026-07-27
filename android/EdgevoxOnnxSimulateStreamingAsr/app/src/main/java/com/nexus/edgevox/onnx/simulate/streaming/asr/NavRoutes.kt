package com.nexus.edgevox.onnx.simulate.streaming.asr

sealed class NavRoutes(val route: String) {
    object Home : NavRoutes("home")
    object Help : NavRoutes("help")
}