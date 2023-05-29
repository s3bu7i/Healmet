import websockets
import asyncio


async def listen():
    url = "https://editor.p5js.org/garciadelcast.."
    async with websockets.connect(url) as ws:
        await ws.send("Hello Server!")
        
        while True:
            msg = await ws.recv()
            print(msg)

asyncio.get_event_loop().run_until_complete(listen())