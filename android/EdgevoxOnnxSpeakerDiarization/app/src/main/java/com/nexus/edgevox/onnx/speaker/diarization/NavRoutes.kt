package com.nexus.edgevox.onnx.speaker.diarization

sealed class NavRoutes(val route: String) {
    object Home : NavRoutes("home")
    object Help : NavRoutes("help")
}