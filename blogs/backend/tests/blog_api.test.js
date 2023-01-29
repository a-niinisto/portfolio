const mongoose = require('mongoose')
const supertest = require('supertest')
const app = require('../app')
const Blog = require('../models/blog')
const User = require('../models/user')

const api = supertest(app)

let initialBlogs = [
    {
        title: 'Kirja',
        author: 'Kirjailija',
        url: 'jotain',
        likes: 15
      },
      {
        title: 'Kirja2',
        author: 'Kirjailija2',
        url: 'jotain2',
        likes: 20
      }
]



describe('GET', () => {
    beforeEach(async () => {
        await Blog.deleteMany({})
        for (let blog of initialBlogs){
            let blogObject = new Blog(blog)
            await blogObject.save()
        }
    })
    test('blogs are returned as json and returns correct amount of them', async () => {
        const response = await api
          .get('/api/blogs')
          .expect(200)
          .expect('Content-Type', /application\/json/)
    
        expect(response.body).toHaveLength(initialBlogs.length)
      })
    
    test('id to be defined', async () => {
        const response = await api
            .get('/api/blogs')
            .expect(200)
        
        expect(response.body[0].id).toBeDefined()
    })
})

describe('POST', () => {
    beforeAll(async () => {
        await User.deleteMany({})
        const userObject = {
            username: "Joku",
            name: "Jotakin",
            password: "Lokki3"
        }
        await api.post('/api/users').send(userObject)
    })
    beforeEach(async () => {
        await Blog.deleteMany({})
        for (let blog of initialBlogs){
            let blogObject = new Blog(blog)
            await blogObject.save()
        }
    })
    test('amount of blogs is increased by one', async () => {
    const newBlog = {
        title: 'Kirja3',
        author: 'Kirjailija3',
        url: 'jotain3',
        likes: 25
      }
    const token = await api.post('/api/login').send({
        username: "Joku",
        password: "Lokki3"
    })
    
    await api.post('/api/blogs')
      .send(newBlog)
      .set('Authorization', "bearer " + token.body.token.toString())
      .expect(201)

    const response = await api.get('/api/blogs').set('Authorization', "bearer " + token.body.token.toString())

    expect(response.body).toHaveLength(initialBlogs.length + 1)

    const vertailtava = response.body.map(blog => {
        return {title: blog.title,
        author: blog.author,
        url: blog.url,
        likes: blog.likes}
    })

    const blogs = expect(vertailtava).toContainEqual(newBlog)

    })

    test('if likes not defined likes to 0', async () => {
        const newBlog = {
            title: 'Kirja3',
            author: 'Kirjailija3',
            url: 'jotain3'
          }
        const token = await  api.post('/api/login').send({
            username: "Joku",
            password: "Lokki3"
        })

        await api.post('/api/blogs')
          .send(newBlog)
          .set('Authorization', "bearer " + token.body.token.toString())
          .expect(201)
    
        const response = await api.get('/api/blogs').set('Authorization', "bearer " + token.body.token.toString())
        const vertailtava = response.body[response.body.length - 1]
        expect(vertailtava.likes).toEqual(0)
    })

    test('if title not set => bad request.', async () => {
        const newBlog = {
            author: 'Kirjailija3',
            url: 'jotain3',
            likes: 10
          }
          const token = await  api.post('/api/login').send({
            username: "Joku",
            password: "Lokki3"
        })
        
        await api.post('/api/blogs')
          .send(newBlog)
          .set('Authorization', "bearer " + token.body.token.toString())
          .expect(400)
    })
    test('if url not set => bad request.', async () => {
        const newBlog = {
            title: "Kirja3",
            author: 'Kirjailija3',
            likes: 10
          }
          const token = await  api.post('/api/login').send({
            username: "Joku",
            password: "Lokki3"
        })
        
        await api.post('/api/blogs')
          .send(newBlog)
          .set('Authorization', "bearer " + token.body.token.toString())
          .expect(400)
    })
    test('if no token returns unauthorized', async () => {
        const newBlog = {
            title: "Kirja3",
            author: 'Kirjailija3',
            likes: 10
          }
          await api.post('/api/blogs')
          .send(newBlog)
          .expect(401)
    })
})
describe('DELETE', () => {
    beforeEach(async () => {
        await Blog.deleteMany({})
        for (let blog of initialBlogs){
            let blogObject = new Blog(blog)
            await blogObject.save()
        }
    })
    test('one can be deleted', async () => {
        let list = await api
            .get('/api/blogs')
        const blogs = list.body
        await api.delete(`/api/blogs/${blogs[0].id}`).expect(204)

        list = await api.get('/api/blogs')
        expect(list.body.length).toEqual(blogs.length - 1)
    })
})

describe('PUT', () => {
    beforeEach(async () => {
        await Blog.deleteMany({})
        for (let blog of initialBlogs){
            let blogObject = new Blog(blog)
            await blogObject.save()
        }
    })
    test('one can be updated', async () => {
        const toUpdate = {
            title: "Kirja3",
            author: "Kirjailija",
            url: "jotakin",
            likes: 10
        }
        let list = await api
            .get('/api/blogs')
        
        const blogs = list.body
        const updatedBlog = await api.put(`/api/blogs/${blogs[0].id}`).send(toUpdate)
        expect({
            title: updatedBlog.body.title,
            author: updatedBlog.body.author,
            url: updatedBlog.body.url,
            likes: updatedBlog.body.likes
        }).toEqual(toUpdate)
        list = await api
            .get('/api/blogs')
        const vertailtava = list.body.map(blog => {
            return {title: blog.title,
            author: blog.author,
            url: blog.url,
            likes: blog.likes}
            })
        expect(vertailtava).toContainEqual(toUpdate)
        
        
    })
})

afterAll(() => {
    mongoose.connection.close()
  })